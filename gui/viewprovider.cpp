#include "viewprovider.h"

#include <Poly_Polygon3D.hxx>
#include <BRepBndLib.hxx>
#include <BRepMesh.hxx>
#include <BRepMesh_IncrementalMesh.hxx>
#include <BRep_Tool.hxx>
#include <BRepTools.hxx>
#include <BRepAdaptor_Curve.hxx>
#include <BRepAdaptor_Surface.hxx>
#include <GeomAbs_CurveType.hxx>
#include <GeomAbs_SurfaceType.hxx>
#include <Geom_BezierCurve.hxx>
#include <Geom_BSplineCurve.hxx>
#include <Geom_BezierSurface.hxx>
#include <Geom_BSplineSurface.hxx>
#include <GeomAPI_ProjectPointOnSurf.hxx>
#include <GeomLProp_SLProps.hxx>
#include <gp_Trsf.hxx>
#include <Poly_Array1OfTriangle.hxx>
#include <Poly_Triangulation.hxx>
#include <TColgp_Array1OfPnt.hxx>
#include <TopoDS.hxx>
#include <TopoDS_Edge.hxx>
#include <TopoDS_Wire.hxx>
#include <TopoDS_Face.hxx>
#include <TopoDS_Iterator.hxx>
#include <TopExp_Explorer.hxx>
#include <TopExp.hxx>
#include <TopTools_IndexedMapOfShape.hxx>
#include <Poly_PolygonOnTriangulation.hxx>
#include <TColStd_Array1OfInteger.hxx>
#include <TopTools_ListOfShape.hxx>
#include <Inventor/SoPickedPoint.h>
#include <Inventor/events/SoMouseButtonEvent.h>
#include <Inventor/nodes/SoCoordinate3.h>
#include <Inventor/nodes/SoDrawStyle.h>
#include <Inventor/nodes/SoIndexedFaceSet.h>
#include <Inventor/nodes/SoLineSet.h>
#include <Inventor/nodes/SoLocateHighlight.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoNormal.h>
#include <Inventor/nodes/SoNormalBinding.h>
#include <Inventor/nodes/SoPointSet.h>
#include <Inventor/nodes/SoPolygonOffset.h>
#include <Inventor/nodes/SoShapeHints.h>
#include <Inventor/nodes/SoSwitch.h>
#include <Inventor/nodes/SoGroup.h>
#include <Inventor/nodes/SoSphere.h>
#include <Inventor/nodes/SoScale.h>
#include <Inventor/nodes/SoSelection.h>

#include "inventorviewer.h"
#include "viewershape.h"

#include <algorithm>

namespace
{
	/*bool operator < (const Gui::ViewProvider::SoShapePair& left, const Gui::ViewProvider::SoShapePair& right)
	{
		return left.first == right.first;
	}

	bool operator < (const Gui::ViewProvider::TopoShapePair& left, const Gui::ViewProvider::TopoShapePair& right)
	{
		return left.first == right.first;
	}*/
}

namespace Gui
{

	ViewProvider::ViewProvider(InventorViewer& viewer) : viewer(viewer)
	{
	}

	ViewProvider::~ViewProvider()
	{
		/*SoShapeList::const_iterator soIt = soShapeList.begin();
		for (; soIt < soShapeList.end(); ++soIt)
			soIt->first->unref();

		TopoShapeList::const_iterator topoIt = topoShapeList.begin();
		for (; topoIt < topoShapeList.end(); ++topoIt)
			topoIt->second->unref();*/
	}

	TopoDS_Shape ViewProvider::getTopoShape(const ViewerShape* shape) const
	{
		std::map<const ViewerShape*, TopoDS_Shape>::const_iterator it = viewerShapeMap.find(shape);
		if (it != viewerShapeMap.end())
			return it->second;

		return TopoDS_Shape();
	}

	const ViewerShape* ViewProvider::getViewerShape(const TopoDS_Shape& shape) const
	{
		TopoShapeList::const_iterator it = findViewerShape(shape);
		if (it != topoShapeList.end())
			return it->second;

		return 0;
	}

	void ViewProvider::display(ViewerShape* shape)
	{
		//addSoShapePair(SoShapePair(group, shape))
		//soShapeMap[group] = shape;

		//TODO: будут ли равны два одинаковых по размеру и положению TopoDS_Shape?
		//topoShapeMap[shape] = group;
		//addTopoShape(TopoShapePair(shape, group));

		viewerShapeMap[shape] = shape->getShape();
		addTopoShapePair(TopoShapePair(shape->getShape(), shape));

		viewer.getRootNode()->addChild(shape->getSoGroup());
	}

	void ViewProvider::remove(ViewerShape* shape)
	{
		/*SoShapeList::const_iterator soIt = findTopoShape(shape);
		assert(soShapeList.end() != soIt);

		TopoShapeList::const_iterator topoIt = findSoShape(soIt->second);
		assert(topoShapeList.end() != topoIt);

		soShapeList.erase(soIt);
		topoShapeList.erase(topoIt);*/

		std::map<const ViewerShape*, TopoDS_Shape>::iterator it = viewerShapeMap.find(shape);
		if (it != viewerShapeMap.end())
		{
			delete it->first;
			viewerShapeMap.erase(it);
		}

		removeTopoShapePair(TopoShapePair(shape->getShape(), shape));

		viewer.getRootNode()->removeChild(shape->getSoGroup());
	}

	ViewProvider::TopoShapeList::const_iterator ViewProvider::findViewerShape(const TopoDS_Shape& shape) const
	{
		return std::find(topoShapeList.begin(), topoShapeList.end(), TopoShapePair(shape, 0));
	}

	ViewProvider::TopoShapeList::iterator ViewProvider::findViewerShape(const TopoDS_Shape& shape)
	{
		return std::find(topoShapeList.begin(), topoShapeList.end(), TopoShapePair(shape, 0));
	}

	void ViewProvider::addTopoShapePair(const TopoShapePair& shapePair)
	{
		TopoShapeList::const_iterator it = findViewerShape(shapePair.first);
		if (it == topoShapeList.end())
			topoShapeList.push_back(shapePair);
	}

	void ViewProvider::removeTopoShapePair(const TopoShapePair& shapePair)
	{
		TopoShapeList::iterator it = findViewerShape(shapePair.first);
		if (it != topoShapeList.end())
			topoShapeList.erase(it);
	}

	/*ViewProvider::SoShapeList::const_iterator ViewProvider::findTopoShape(const SoGroup* shape)
	{
		return std::find(soShapeList.begin(), soShapeList.end(), SoShapePair(shape, TopoDS_Shape()));
	}

	ViewProvider::TopoShapeList::const_iterator ViewProvider::findSoShape(const TopoDS_Shape& shape)
	{
		return std::find(topoShapeList.begin(), topoShapeList.end(), TopoShapePair(shape, 0));
	}

	void ViewProvider::addSoShapePair(const SoShapePair& shapePair)
	{
	}

	void ViewProvider::addTopoShapePair(const TopoShapePair& shapePair)
	{
	}

	void ViewProvider::removeSoShapePair(const SoShapePair& shapePair)
	{
	}

	void ViewProvider::removeTopoShapePair(const TopoShapePair& shapePair)
	{
	}*/

	void ViewProvider::computeEdges(SoGroup* edgeRoot, const TopoDS_Shape &shape)
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

			// define the indexed face set
			/*Gui::SoFCSelection* sel = createFromSettings();
			SbString name("Edge");
			name += SbString(i+1);
			sel->objectName = pcObject->getNameInDocument();
			sel->documentName = pcObject->getDocument()->getName();
			sel->subElementName = name;
			sel->style = Gui::SoFCSelection::EMISSIVE_DIFFUSE;
			//sel->highlightMode = Gui::SoFCSelection::AUTO;
			//sel->selectionMode = Gui::SoFCSelection::SEL_ON;*/

			SoLocateHighlight* h = new SoLocateHighlight();

			SoLineSet * lineset = new SoLineSet;
			h->addChild(lineset);
			edgeRoot->addChild(h);
			//vertexShapeMap[lineset] = aEdge;
		}
	}

	void ViewProvider::computeVertices(SoGroup* vertexRoot, const TopoDS_Shape &shape)
	{
		//vertexRoot->addChild(pcPointMaterial);  
		//vertexRoot->addChild(pcPointStyle);

		// get a indexed map of edges
		TopTools_IndexedMapOfShape M;
		TopExp::MapShapes(shape, TopAbs_VERTEX, M);
		
		for (int i=0; i<M.Extent(); i++)
		{
			const TopoDS_Vertex& aVertex = TopoDS::Vertex(M(i+1));

			// each point has its own selection node
			/*Gui::SoFCSelection* sel = createFromSettings();
			SbString name("Point");
			name += SbString(i+1);
			sel->objectName = pcObject->getNameInDocument();
			sel->documentName = pcObject->getDocument()->getName();
			sel->subElementName = name;
			sel->style = Gui::SoFCSelection::EMISSIVE_DIFFUSE;
			//sel->highlightMode = Gui::SoFCSelection::AUTO;
			//sel->selectionMode = Gui::SoFCSelection::SEL_ON;*/

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
			/*else if ((*vertexnormals)[i].sqrLength() > 0.001)
			{
				(*vertexnormals)[i].normalize();
			}*/
		}
	}

	void ViewProvider::computeFaces(SoGroup* faceRoot, const TopoDS_Shape &shape, double deflection)
	{
		TopExp_Explorer ex;

		//faceRoot->addChild(pcShapeMaterial);

	//  BRepMesh::Mesh(shape,1.0);
		BRepMesh_IncrementalMesh MESH(shape,deflection);

		int i = 1;
		for (ex.Init(shape, TopAbs_FACE); ex.More(); ex.Next(),i++)
		{
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

			//if (!this->noPerVertexNormals)
			{
				// define normals (this is optional)
				SoNormal * norm = new SoNormal;
				norm->vector.setValues(0, nbNodesInFace, vertexnormals);
				faceRoot->addChild(norm);

				// bind one normal per face
				SoNormalBinding * normb = new SoNormalBinding;
				normb->value = SoNormalBinding::PER_VERTEX_INDEXED;
				faceRoot->addChild(normb);
			}

			// define vertices
			SoCoordinate3 * coords = new SoCoordinate3;
			coords->point.setValues(0,nbNodesInFace, vertices);
			faceRoot->addChild(coords);

			// Turns on backface culling
			//      SoShapeHints * hints = new SoShapeHints;
			//      hints->vertexOrdering = SoShapeHints::CLOCKWISE ;
			//      hints->vertexOrdering = SoShapeHints::COUNTERCLOCKWISE ;
			//      hints->shapeType = SoShapeHints::SOLID;
			//      hints->shapeType = SoShapeHints::UNKNOWN_SHAPE_TYPE;
			//      root->addChild(hints);

			//SoDrawStyle *Stype = new SoDrawStyle();
			//Stype->pointSize.setValue(3.0);
			//Stype->style.setValue( SoDrawStyle::POINTS );

			//SoPointSet *PtSet = new SoPointSet;
			//root->addChild(PtSet);

			// define the indexed face set
			/*Gui::SoFCSelection* sel = createFromSettings();
			SbString name("Face");
			name += SbString(i);
			sel->objectName = pcObject->getNameInDocument();
			sel->documentName = pcObject->getDocument()->getName();
			sel->subElementName = name;
			sel->style = Gui::SoFCSelection::EMISSIVE;
			//sel->highlightMode = Gui::SoFCSelection::AUTO;
			//sel->selectionMode = Gui::SoFCSelection::SEL_ON;*/

			SoIndexedFaceSet * faceset = new SoIndexedFaceSet;
			faceset->coordIndex.setValues(0,4*nbTriInFace,(const int32_t*) cons);
			SoLocateHighlight* h = new SoLocateHighlight();
			h->addChild(faceset);
			faceRoot->addChild(h);
			//soShapeMap[faceset] = aFace;

			delete [] vertexnormals;
			delete [] vertices;
			delete [] cons;
		} // end of face loop
	}

}

