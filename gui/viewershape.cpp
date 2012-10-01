#include "viewershape.h"

#include "viewprovider.h"

#include <BRepAdaptor_Curve.hxx>
#include <BRepAdaptor_Surface.hxx>
#include <BRepBndLib.hxx>
#include <BRepMesh.hxx>
#include <BRepMesh_IncrementalMesh.hxx>
#include <BRepTools.hxx>
#include <BRep_Tool.hxx>
#include <Bnd_Box.hxx>
#include <GeomAPI_ProjectPointOnSurf.hxx>
#include <GeomAbs_CurveType.hxx>
#include <GeomAbs_SurfaceType.hxx>
#include <GeomLProp_SLProps.hxx>
#include <Geom_BSplineCurve.hxx>
#include <Geom_BSplineSurface.hxx>
#include <Geom_BezierCurve.hxx>
#include <Geom_BezierSurface.hxx>
#include <Inventor/SoPath.h>
#include <Inventor/actions/SoSearchAction.h>
#include <Inventor/events/SoMouseButtonEvent.h>
#include <Inventor/manips/SoCenterballManip.h>
#include <Inventor/nodes/SoCoordinate3.h>
#include <Inventor/nodes/SoDrawStyle.h>
#include <Inventor/nodes/SoGroup.h>
#include <Inventor/nodes/SoIndexedFaceSet.h>
#include <Inventor/nodes/SoLineSet.h>
#include <Inventor/nodes/SoLocateHighlight.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoNormal.h>
#include <Inventor/nodes/SoNormalBinding.h>
#include <Inventor/nodes/SoPointSet.h>
#include <Inventor/nodes/SoPolygonOffset.h>
#include <Inventor/nodes/SoScale.h>
#include <Inventor/nodes/SoSelection.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoShapeHints.h>
#include <Inventor/nodes/SoSphere.h>
#include <Inventor/nodes/SoSwitch.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/sensors/SoNodeSensor.h>
#include <Poly_Array1OfTriangle.hxx>
#include <Poly_Polygon3D.hxx>
#include <Poly_PolygonOnTriangulation.hxx>
#include <Poly_Triangulation.hxx>
#include <TColStd_Array1OfInteger.hxx>
#include <TColgp_Array1OfPnt.hxx>
#include <TopExp.hxx>
#include <TopExp_Explorer.hxx>
#include <TopTools_IndexedMapOfShape.hxx>
#include <TopTools_ListOfShape.hxx>
#include <TopoDS.hxx>
#include <TopoDS_Edge.hxx>
#include <TopoDS_Face.hxx>
#include <TopoDS_Iterator.hxx>
#include <TopoDS_Wire.hxx>
#include <gp_Trsf.hxx>

#include <cassert>

namespace Gui
{

	ViewerShape::ViewerShape(QString name, Shape shape) :
		shape(shape), name(name)
	{
		rootGroup = new SoGroup();
		rootGroup->ref();

		rootSeparator = new SoSeparator();
		rootSeparator->ref();
		rootGroup->addChild(rootSeparator);

		transform = new SoTransform();
		transform->ref();
		rootSeparator->addChild(transform);

		computeShape();
	}

	ViewerShape::~ViewerShape()
	{
		removeManip();
		rootGroup->unref();

		transform->unref();
		rootSeparator->unref();
	}

	QString ViewerShape::getName() const
	{
		return name;
	}

	SoGroup* ViewerShape::getSoGroup() const
	{
		return rootGroup;
	}

	Shape ViewerShape::getShape() const
	{
		return shape;
	}

	void ViewerShape::setCentralBallManip() const
	{
		SoSearchAction sa;
		sa.setInterest(SoSearchAction::FIRST);
		sa.setSearchingAll(FALSE);
		sa.setNode(transform);
		sa.apply(rootGroup);
		SoPath* path = sa.getPath();
		if (!path)
			return;

		SoCenterballManip* manip = new SoCenterballManip();
		SoNodeSensor* sensor = new SoNodeSensor(transformCallback, const_cast<ViewerShape*>(this));
		sensor->attach(manip);
		manip->setUserData(sensor);

		manip->replaceNode(path);
	}

	void ViewerShape::removeManip() const
	{
		SoSearchAction sa;
		sa.setType(SoCenterballManip::getClassTypeId());
		sa.setInterest(SoSearchAction::FIRST);
		sa.apply(rootGroup);
		SoPath * path = sa.getPath();

		if (!path)
			return;

		SoCenterballManip* manip = static_cast<SoCenterballManip*>(path->getTail());
		SoNodeSensor* sensor = reinterpret_cast<SoNodeSensor*>(manip->getUserData());

		sensor->detach();
		delete sensor;

		manip->replaceManip(path, transform);
	}

	TopoDS_Shape ViewerShape::getShape(const SoPath* path) const
	{
		if (path)
		{
			if (path->getTail()->getTypeId().isDerivedFrom(SoVertexShape::getClassTypeId()))
			{
				SoVertexShape* vertexShape = static_cast<SoVertexShape*>(path->getTail());
				assert(vertexShape);
				std::map<SoVertexShape*, TopoDS_Shape>::const_iterator it = shapeMap.find(vertexShape);
				if (it != shapeMap.end())
					return it->second;
			}
		}

		return TopoDS_Shape();
	}

	void ViewerShape::computeShape()
	{
		Bnd_Box bounds;
		BRepBndLib::Add(shape.getShape(), bounds);
		bounds.SetGap(0.0);
		Standard_Real xMin, yMin, zMin, xMax, yMax, zMax;
		bounds.Get(xMin, yMin, zMin, xMax, yMax, zMax);
		Standard_Real deflection = ((xMax-xMin)+(yMax-yMin)+(zMax-zMin))/300.0 *
			0.2;
		BRepMesh::Mesh(shape.getShape(), deflection);

		faces = new SoGroup();

		computeFaces(faces, shape.getShape(), deflection);

		rootSeparator->addChild(faces);

		edges = new SoGroup();
		computeEdges(edges, shape.getShape());

		rootSeparator->addChild(edges);

		vertices = new SoGroup();
		computeVertices(vertices, shape.getShape());

		rootSeparator->addChild(vertices);
	}

	void ViewerShape::transformCallback(void* userData, SoSensor* sensor)
	{
		ViewerShape* self = reinterpret_cast<ViewerShape*>(userData);
		assert(self);

		SoNodeSensor* nodeSensor = static_cast<SoNodeSensor*>(sensor);
		SoNode* node = nodeSensor->getAttachedNode();

		if (node && node->getTypeId().isDerivedFrom(SoCenterballManip::getClassTypeId()))
		{
			SoCenterballManip* manip = static_cast<SoCenterballManip*>(node);
			float q0, q1, q2, q3;
			SbVec3f move = manip->translation.getValue();
			SbVec3f center = manip->center.getValue();
			manip->rotation.getValue().getValue(q0, q1, q2, q3);
			SbRotation rotation(q0, q1, q2, q3);

			gp_Trsf shapeTrsfm = self->shape.getTransform();

			SbMatrix matrix(shapeTrsfm.Value(1, 1), shapeTrsfm.Value(1, 2), shapeTrsfm.Value(1, 3), shapeTrsfm.Value(1, 4),
							shapeTrsfm.Value(2, 1), shapeTrsfm.Value(2, 2), shapeTrsfm.Value(2, 3), shapeTrsfm.Value(2, 4), 
							shapeTrsfm.Value(3, 1), shapeTrsfm.Value(3, 2), shapeTrsfm.Value(3, 3), shapeTrsfm.Value(3, 4),
							0, 0, 0, 0);

			//const SbMat& mat_old = matrix.getValue();

			/*qDebug() << "old matrix";
			qDebug() << mat_old[0][0] << mat_old[0][1] << mat_old[0][2] << mat_old[0][3];
			qDebug() << mat_old[1][0] << mat_old[1][1] << mat_old[1][2] << mat_old[1][3];
			qDebug() << mat_old[2][0] << mat_old[2][1] << mat_old[2][2] << mat_old[2][3];
			qDebug() << "";*/

			SbMatrix rotationMatrix;
			rotationMatrix.setRotate(rotation);
			matrix.multRight(rotationMatrix);
			//matrix.setTranslate(move);

			const SbMat& mat = matrix.getValue();

			//qDebug() << "new matrix";
			gp_Trsf newTrsfm;
			newTrsfm.SetValues(mat[0][0], mat[0][1], mat[0][2], mat[0][3],
								mat[1][0], mat[1][1], mat[1][2], mat[1][3], 	
								mat[2][0], mat[2][1], mat[2][2], mat[2][3],
								0.00001, 0.00001);

			self->shape.setTransform(newTrsfm);

			//qDebug() << q0 << q1 << q2 << q3;

			/*qDebug() << mat[0][0] << mat[0][1] << mat[0][2] << mat[0][3];
			qDebug() << mat[1][0] << mat[1][1] << mat[1][2] << mat[1][3];
			qDebug() << mat[2][0] << mat[2][1] << mat[2][2] << mat[2][3];
			qDebug() << "";*/

			/*SbRotation rotation(q0, q1, q2, q3);
			SbVec3f m(move[0], move[1], move[2]);
			SbVec3f c(center[0], center[1], center[2]);
			m += c;
			rotation.multVec(c, c);
			m -= c;*/
		}
	}

	void ViewerShape::computeEdges(SoGroup* edgeRoot, const TopoDS_Shape &shape)
	{
		//edgeRoot->addChild(pcLineMaterial);  
		//edgeRoot->addChild(pcLineStyle);

		// get a indexed map of edges
		TopTools_IndexedMapOfShape M;
		TopExp::MapShapes(shape, TopAbs_EDGE, M);

		// build up map edge->face
		TopTools_IndexedDataMapOfShapeListOfShape edge2Face;
		TopExp::MapShapesAndAncestors(shape, TopAbs_EDGE, TopAbs_FACE, edge2Face);

		for (int i = 0; i < M.Extent(); i++)
		{
			// get the shape and mesh it
			const TopoDS_Edge& aEdge = TopoDS::Edge(M(i+1));

			// getting the transformation of the shape/face
			gp_Trsf myTransf;
			Standard_Boolean identity = true;
			TopLoc_Location aLoc;

			// try to triangulate the edge
			Handle(Poly_Polygon3D) aPoly = BRep_Tool::Polygon3D(aEdge, aLoc);

			SbVec3f* vertices;
			Standard_Integer nbNodesInFace;

			// triangulation succeeded?
			if (!aPoly.IsNull())
			{
				if (!aLoc.IsIdentity())
				{
					identity = false;
					myTransf = aLoc.Transformation();
				}
				// take the edge's triangulation
				//
				// getting size and create the array
				nbNodesInFace = aPoly->NbNodes();
				vertices = new SbVec3f[nbNodesInFace];

				const TColgp_Array1OfPnt& Nodes = aPoly->Nodes();

				gp_Pnt V;
				for (Standard_Integer i=0;i < nbNodesInFace;i++)
				{
					V = Nodes(i+1);
					V.Transform(myTransf);
					vertices[i].setValue((float)(V.X()),(float)(V.Y()),(float)(V.Z()));
				}
			}
			else
			{
				// the edge has not its own triangulation, but then a face the edge is attached to
				// must provide this triangulation

				// Look for one face in our map (it doesn't care which one we take)
				const TopoDS_Face& aFace = TopoDS::Face(edge2Face.FindFromKey(aEdge).First());

				// take the face's triangulation instead
				Handle(Poly_Triangulation) aPolyTria = BRep_Tool::Triangulation(aFace,aLoc);
				if (!aLoc.IsIdentity())
				{
					identity = false;
					myTransf = aLoc.Transformation();
				}

				//if (aPolyTria.IsNull()) // actually this shouldn't happen at all
				//    throw Base::Exception("Empty face trianglutaion\n");
				if (aPolyTria.IsNull()) return;

				// this holds the indices of the edge's triangulation to the actual points
				Handle(Poly_PolygonOnTriangulation) aPoly = BRep_Tool::PolygonOnTriangulation(aEdge, aPolyTria, aLoc);
				if (aPoly.IsNull())
					continue; // polygon does not exist

				// getting size and create the array
				nbNodesInFace = aPoly->NbNodes();
				vertices = new SbVec3f[nbNodesInFace];

				const TColStd_Array1OfInteger& indices = aPoly->Nodes();
				const TColgp_Array1OfPnt& Nodes = aPolyTria->Nodes();

				gp_Pnt V;
				int pos = 0;
				// go through the index array
				for (Standard_Integer i=indices.Lower();i <= indices.Upper();i++)
				{
					V = Nodes(indices(i));
					V.Transform(myTransf);
					vertices[pos++].setValue((float)(V.X()),(float)(V.Y()),(float)(V.Z()));
				}
			}

			// define vertices
			SoCoordinate3 * coords = new SoCoordinate3;
			coords->point.setValues(0,nbNodesInFace, vertices);
			delete [] vertices;
			edgeRoot->addChild(coords);

			SoLocateHighlight* h = new SoLocateHighlight();

			SoLineSet * lineset = new SoLineSet;
			h->addChild(lineset);
			edgeRoot->addChild(h);
			shapeMap[lineset] = aEdge;
		}
	}

	void ViewerShape::computeVertices(SoGroup* vertexRoot, const TopoDS_Shape &shape)
	{
		//vertexRoot->addChild(pcPointMaterial);  
		//vertexRoot->addChild(pcPointStyle);

		// get a indexed map of edges
		TopTools_IndexedMapOfShape M;
		TopExp::MapShapes(shape, TopAbs_VERTEX, M);
		
		for (int i=0; i<M.Extent(); i++)
		{
			const TopoDS_Vertex& aVertex = TopoDS::Vertex(M(i+1));

			// define the vertices
			SoCoordinate3 * coords = new SoCoordinate3;
			coords->point.setNum(1);
			vertexRoot->addChild(coords);

			// get the shape
			gp_Pnt pnt = BRep_Tool::Pnt(aVertex);
			coords->point.set1Value(0, (float)pnt.X(), (float)pnt.Y(), (float)pnt.Z());

			SoLocateHighlight* h = new SoLocateHighlight();

			SoPointSet * pointset = new SoPointSet;
			h->addChild(pointset);
			vertexRoot->addChild(h);
		}
	}

	void transferToArray(const TopoDS_Face& aFace,SbVec3f** vertices,SbVec3f** vertexnormals,
										   int32_t** cons,int &nbNodesInFace,int &nbTriInFace )
	{
		TopLoc_Location aLoc;

		// doing the meshing and checking the result
		//BRepMesh_IncrementalMesh MESH(aFace,fdeflectionection);
		Handle(Poly_Triangulation) aPoly = BRep_Tool::Triangulation(aFace,aLoc);
		//if (aPoly.IsNull()) throw Base::Exception("Empty face trianglutaion\n");
		if (aPoly.IsNull()) return;

		// getting the transformation of the shape/face
		gp_Trsf myTransf;
		Standard_Boolean identity = true;
		if (!aLoc.IsIdentity())
		{
			identity = false;
			myTransf = aLoc.Transformation();
		}

		Standard_Integer i;
		// geting size and create the array
		nbNodesInFace = aPoly->NbNodes();
		nbTriInFace = aPoly->NbTriangles();
		*vertices = new SbVec3f[nbNodesInFace];
		*vertexnormals = new SbVec3f[nbNodesInFace];
		for(i=0;i < nbNodesInFace;i++)
		{
			(*vertexnormals)[i]= SbVec3f(0.0,0.0,0.0);
		}

		*cons = new int32_t[4*(nbTriInFace)];

		// check orientation
		TopAbs_Orientation orient = aFace.Orientation();

		// cycling through the poly mesh
		const Poly_Array1OfTriangle& Triangles = aPoly->Triangles();
		const TColgp_Array1OfPnt& Nodes = aPoly->Nodes();
		for (i=1;i<=nbTriInFace;i++)
		{
			// Get the triangle
			Standard_Integer N1,N2,N3;
			Triangles(i).Get(N1,N2,N3);

			// change orientation of the triangles
			if ( orient != TopAbs_FORWARD )
			{
				Standard_Integer tmp = N1;
				N1 = N2;
				N2 = tmp;
			}

			gp_Pnt V1 = Nodes(N1);
			gp_Pnt V2 = Nodes(N2);
			gp_Pnt V3 = Nodes(N3);

			// transform the vertices to the place of the face
			if (!identity)
			{
				V1.Transform(myTransf);
				V2.Transform(myTransf);
				V3.Transform(myTransf);
			}

			//if (!this->noPerVertexNormals)
			{
				// Calculate triangle normal
				gp_Vec v1(V1.X(),V1.Y(),V1.Z()),v2(V2.X(),V2.Y(),V2.Z()),v3(V3.X(),V3.Y(),V3.Z());
				gp_Vec Normal = (v2-v1)^(v3-v1); 

				//Standard_Real Area = 0.5 * Normal.Magnitude();

				// add the triangle normal to the vertex normal for all points of this triangle
				(*vertexnormals)[N1-1] += SbVec3f(Normal.X(),Normal.Y(),Normal.Z());
				(*vertexnormals)[N2-1] += SbVec3f(Normal.X(),Normal.Y(),Normal.Z());
				(*vertexnormals)[N3-1] += SbVec3f(Normal.X(),Normal.Y(),Normal.Z());
			}

			(*vertices)[N1-1].setValue((float)(V1.X()),(float)(V1.Y()),(float)(V1.Z()));
			(*vertices)[N2-1].setValue((float)(V2.X()),(float)(V2.Y()),(float)(V2.Z()));
			(*vertices)[N3-1].setValue((float)(V3.X()),(float)(V3.Y()),(float)(V3.Z()));

			int j = i - 1;
			N1--; N2--; N3--;
			(*cons)[4*j] = N1; (*cons)[4*j+1] = N2; (*cons)[4*j+2] = N3; (*cons)[4*j+3] = SO_END_FACE_INDEX;
		}

		// normalize all vertex normals
		for(i=0;i < nbNodesInFace;i++)
		{
			//if (this->qualityNormals)
			{
				gp_Dir clNormal;

				try
				{
					Handle_Geom_Surface Surface = BRep_Tool::Surface(aFace);

					gp_Pnt vertex((*vertices)[i][0], (*vertices)[i][1], (*vertices)[i][2]);
					GeomAPI_ProjectPointOnSurf ProPntSrf(vertex, Surface);
					Standard_Real fU, fV; ProPntSrf.Parameters(1, fU, fV);

					GeomLProp_SLProps clPropOfFace(Surface, fU, fV, 2, gp::Resolution());

					clNormal = clPropOfFace.Normal();
					SbVec3f temp = SbVec3f(clNormal.X(),clNormal.Y(),clNormal.Z());
					//Base::Console().Log("unterschied:%.2f",temp.dot((*vertexnormals)[i]));

					if ( temp.dot((*vertexnormals)[i]) < 0 )
						temp = -temp;
					(*vertexnormals)[i] = temp;

				}
				catch(...){}
			}
		}
	}

	void ViewerShape::computeFaces(SoGroup* faceRoot, const TopoDS_Shape &shape, double deflection)
	{
		TopExp_Explorer ex;

		BRepMesh_IncrementalMesh MESH(shape,deflection);

		int i = 1;
		for (ex.Init(shape, TopAbs_FACE); ex.More(); ex.Next(),i++) {
			// get the shape and mesh it
			const TopoDS_Face& aFace = TopoDS::Face(ex.Current());


			// this block mesh the face and transfers it in a C array of vertices and face indexes
			Standard_Integer nbNodesInFace,nbTriInFace;
			SbVec3f* vertices=0;
			SbVec3f* vertexnormals=0;
			int32_t* cons=0;

			transferToArray(aFace,&vertices,&vertexnormals,&cons,nbNodesInFace,nbTriInFace);

			if (!vertices)
				continue;

			/*if (!this->noPerVertexNormals) {
				// define normals (this is optional)
				SoNormal * norm = new SoNormal;
				norm->vector.setValues(0, nbNodesInFace, vertexnormals);
				FaceRoot->addChild(norm);

				// bind one normal per face
				SoNormalBinding * normb = new SoNormalBinding;
				normb->value = SoNormalBinding::PER_VERTEX_INDEXED;
				FaceRoot->addChild(normb);
			}*/

			// define vertices
			SoCoordinate3 * coords = new SoCoordinate3;
			coords->point.setValues(0,nbNodesInFace, vertices);
			faceRoot->addChild(coords);


			SoIndexedFaceSet * faceset = new SoIndexedFaceSet;
			faceset->coordIndex.setValues(0,4*nbTriInFace,(const int32_t*) cons);
			SoLocateHighlight* sel = new SoLocateHighlight();
			sel->addChild(faceset);
			faceRoot->addChild(sel);
			shapeMap[faceset] = aFace;

			delete [] vertexnormals;
			delete [] vertices;
			delete [] cons;
		} // end of face loop
	}

}

