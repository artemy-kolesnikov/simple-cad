#include "viewershape.h"

#include "viewprovider.h"

#include <Inventor/nodes/SoGroup.h>
#include <BRepBndLib.hxx>
#include <Bnd_Box.hxx>
#include <BRepMesh.hxx>

namespace Gui
{

	ViewerShape::ViewerShape(QString name, TopoDS_Shape shape) :
		name(name), shape(shape)
	{
		rootGroup = new SoGroup();
		rootGroup->ref();

		computeShape();
	}

	ViewerShape::~ViewerShape()
	{
		faces->unref();
		edges->unref();
		vertices->unref();
		rootGroup->unref();
	}

	QString ViewerShape::getName() const
	{
		return name;
	}

	SoGroup* ViewerShape::getSoGroup() const
	{
		return rootGroup;
	}

	TopoDS_Shape ViewerShape::getShape() const
	{
		return shape;
	}

	void ViewerShape::computeShape()
	{
		Bnd_Box bounds;
		BRepBndLib::Add(shape, bounds);
		bounds.SetGap(0.0);
		Standard_Real xMin, yMin, zMin, xMax, yMax, zMax;
		bounds.Get(xMin, yMin, zMin, xMax, yMax, zMax);
		Standard_Real deflection = ((xMax-xMin)+(yMax-yMin)+(zMax-zMin))/300.0 *
			0.2;
		BRepMesh::Mesh(shape,deflection);

		faces = new SoGroup();
		faces->ref();

		ViewProvider::computeFaces(faces, shape, deflection);

		rootGroup->addChild(faces);

		edges = new SoGroup();
		edges->ref();
		ViewProvider::computeEdges(edges, shape);

		rootGroup->addChild(edges);

		vertices = new SoGroup();
		vertices->ref();
		ViewProvider::computeVertices(vertices, shape);

		rootGroup->addChild(vertices);
	}

}
