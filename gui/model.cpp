/*
 * Simple CAD System
 *
 * Copyright (C) 2010 Artemy Kolesnikov <artemy.kolesnikov@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include "model.h"
#include "filehelper.h"

#include <QApplication>
#include <QDebug>

#include <algorithm>
#include <stdlib.h>

#include <BRepAlgoAPI_Common.hxx>
#include <BRepAlgoAPI_Cut.hxx>
#include <BRepAlgoAPI_Fuse.hxx>
#include <BRepBuilderAPI_GTransform.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <BRepBuilderAPI_MakeWire.hxx>
#include <BRepPrimAPI_MakeBox.hxx>
#include <BRepPrimAPI_MakeCone.hxx>
#include <BRepPrimAPI_MakeCylinder.hxx>
#include <BRepPrimAPI_MakePrism.hxx>
#include <BRepPrimAPI_MakeSphere.hxx>
#include <BRepPrimAPI_MakeTorus.hxx>
#include <GC_MakePlane.hxx>
#include <GC_MakeSegment.hxx>
#include <Geom_Plane.hxx>
#include <Geom_TrimmedCurve.hxx>
#include <TCollection_ExtendedString.hxx>
#include <TopTools_HSequenceOfShape.hxx>
#include <TopoDS_Edge.hxx>
#include <TopoDS_Face.hxx>
#include <TopoDS_Wire.hxx>
#include <TopoDS_Solid.hxx>
#include <gp_GTrsf.hxx>
#include <gp_Pln.hxx>
#include <gp_Vec.hxx>

#include <Inventor/Qt/viewers/SoQtPlaneViewer.h>
#include <Inventor/nodes/SoCamera.h>
#include <Inventor/nodes/SoSeparator.h>

#include <exception.h>
#include <viewprovider.h>
#include <shapemodel.h>

#include <createcone.h>
#include <createbox.h>
#include <createcylinder.h>

namespace Gui
{

	Model::Model(QObject* parent) : QObject(parent), nextShapeId(0)
	{
		shapeModel.reset(new ShapeModel(this));
	}

	Model::~Model()
	{
	}

	void Model::load(QString& fileName)
	{
		std::list<TopoDS_Shape> shapes;
		FileHelper::readFile(fileName, shapes);

		std::list<TopoDS_Shape>::const_iterator it = shapes.begin();
		for (; it != shapes.end(); ++it)
			addShape(Shape(*it));

		this->fileName = fileName;

		Q_EMIT fileNameChanged(fileName);
	}

	void Model::save(QString& fileName)
	{
		/*if (!fileName.isEmpty())
		{
			this->fileName = fileName;
			Q_EMIT fileNameChanged(fileName);
		}*/
	}

	QString Model::getFileName() const
	{
		return fileName;
	}

	const QList<Shape>& Model::getShapes() const
	{
		return shapeList;
	}

	void Model::removeShape(const Shape& shape)
	{
		// TODO: Удаление шейпа из коллекции
		int index = shapeList.indexOf(shape);
		if (index != -1)
		{
			Q_EMIT shapeRemoved(shape);
			shapeList.removeAt(index);
		}
	}

	void Model::addShape(const Shape& shape)
	{
		shapeList.push_back(shape);
		Q_EMIT shapeAdded(shape);
	}

	ShapeModel* Model::getQModel() const
	{
		return shapeModel.get();
	}

}

