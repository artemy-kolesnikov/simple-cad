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
#include <gp_GTrsf.hxx>
#include <gp_Pln.hxx>
#include <gp_Vec.hxx>

#include <Inventor/Qt/viewers/SoQtPlaneViewer.h>
#include <Inventor/nodes/SoCamera.h>
#include <Inventor/nodes/SoSeparator.h>

#include <exception.h>
#include <viewprovider.h>

namespace Gui
{

	Model::Model(QObject* parent) : QObject(parent)
	{
		shapeList = new TopTools_HSequenceOfShape();
	}

	void Model::loadModel(QString& fileName)
	{
		FileHelper::readFile(fileName, shapeList);

		if (shapeList.IsNull() || !shapeList->Length())
			throw Common::FileException(QObject::tr("Ошибка чтения элементов"));

		/*ViewProvider provider;

		for (int i = 1; i <= dsShapes->Length(); ++i)
		{
			SoGroup* faces = new SoGroup();
			faces->ref();
			//SoGroup* vert = new SoGroup();
			//vert->ref();
			//SoGroup* edges = new SoGroup();
			//edges->ref();

			provider.computeFaces(faces, dsShapes->Value(i), .2);
			//computeVertices(vert, dsShapes->Value(i));
			//computeEdges(edges, dsShapes->Value(i));

			separator->addChild(faces);

			SbViewportRegion region(viewer->getSize());
		
			viewer->getCamera()->viewAll(faces, region);
			//separator->addChild(vert);
			//separator->addChild(edges);
		}*/

		this->fileName = fileName;

		/*Q_EMIT changed();
		Q_EMIT fileNameChanged(fileName);*/
	}

	void Model::saveModel(QString& fileName)
	{
		if (!fileName.isEmpty())
		{
			this->fileName = fileName;
			Q_EMIT fileNameChanged(fileName);
		}
	}

	QString Model::getFileName() const
	{
		return fileName;
	}

	const TopTools_HSequenceOfShape& Model::getShapes() const
	{
		//return *shapeList;
	}

	/*boost::shared_ptr<AIS_SequenceOfInteractive> Model::getSelectedShapes() const
	{
		boost::shared_ptr<AIS_SequenceOfInteractive> selected(new AIS_SequenceOfInteractive());

		for (context->InitSelected(); context->MoreSelected(); context->NextSelected())
		{
			Handle(AIS_InteractiveObject) object = new AIS_Shape(context->SelectedShape());

			//if (object->IsKind(STANDARD_TYPE(AIS_Shape)))
				selected->Append(object);
		}

		return selected;
	}

	boost::shared_ptr<AIS_SequenceOfInteractive> Model::getCurrentShapes() const
	{
		boost::shared_ptr<AIS_SequenceOfInteractive> currentList(new AIS_SequenceOfInteractive());

		for (context->InitCurrent(); context->MoreCurrent(); context->NextCurrent())
		{
			Handle(AIS_InteractiveObject) object = context->Current();

			//if (object->IsKind(STANDARD_TYPE(AIS_Shape)))
				currentList->Append(object);
		}

		return currentList;
	}

	void Model::setMaterial(Graphic3d_NameOfMaterial material)
	{
		for (context->InitCurrent(); context->MoreCurrent(); context->NextCurrent())
		{
			Handle(AIS_InteractiveObject) object = context->Current();

			context->SetMaterial(object, material);
		}
	}

	void Model::setMaterial(const QString& material)
	{
		Graphic3d_NameOfMaterial material_type = getMaterialType(material);

		setMaterial(material_type);
	}

	void Model::setShadded(bool shadded)
	{
		for (context->InitCurrent(); context->MoreCurrent(); context->NextCurrent())
		{
			Handle(AIS_InteractiveObject) object = context->Current();

			context->SetDisplayMode(object, shadded, true);
		}
	}*/

	void Model::removeShape(const TopoDS_Shape& shape)
	{
		// TODO: Удаление шейпа из коллекции
		//context->Erase(shape);
		Q_EMIT shapeRemoved(shape);
	}

	void Model::addShape(const TopoDS_Shape& shape)
	{
		Q_EMIT shapeAdded(shape);
	}

}

