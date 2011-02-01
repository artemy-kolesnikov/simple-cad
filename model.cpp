/*
 * Simple CAD System
 * Based on opencascade (www.opencascade.org)
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

#include <stdlib.h>
#include <algorithm>

#include <QDebug>
#include <QApplication>

#include <Graphic3d_GraphicDevice.hxx>
#include <TopTools_HSequenceOfShape.hxx>
#include <Geom_TrimmedCurve.hxx>
#include <GC_MakeSegment.hxx>
#include <TopoDS_Edge.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <BRepBuilderAPI_MakeWire.hxx>
#include <TopoDS_Wire.hxx>
#include <TopoDS_Face.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <gp_Vec.hxx>
#include <BRepPrimAPI_MakePrism.hxx>
#include <AIS_Shape.hxx>

namespace
{

	static Handle(Graphic3d_GraphicDevice) defaultDevice;

	const int MATERIALS_COUNT = 20;
	const QString material_names[MATERIALS_COUNT] = 
	{
		QObject::tr("Brass"),//("Глина"),							// Graphic3d_NOM_BRASS
		QObject::tr("Bronze"),//("Бронза"),							// Graphic3d_NOM_BRONZE
		QObject::tr("Copper"),//("Медь"),							// Graphic3d_NOM_COPPER
		QObject::tr("Gold"),//("Золото"),							// Graphic3d_NOM_GOLD
		QObject::tr("Pewter"),//("Олово"),							// Graphic3d_NOM_PEWTER
		QObject::tr("Plaster"),//("Гипс"),							// Graphic3d_NOM_PLASTER
		QObject::tr("Plastic"),//("Пластик"),							// Graphic3d_NOM_PLASTIC
		QObject::tr("Silver"),//("Серебро"),							// Graphic3d_NOM_SILVER
		QObject::tr("Steel"),//("Сталь"),							// Graphic3d_NOM_STEEL
		QObject::tr("Stone"),//("Камень"),							// Graphic3d_NOM_STONE
		QObject::tr("Shiny plastic"),//("Блестящий пластик"),				// Graphic3d_NOM_SHINY_PLASTIC
		QObject::tr("Satin"),//("Атлас"),							// Graphic3d_NOM_SATIN
		QObject::tr("Metalized"),//("Металлизированная поверхность"),	// Graphic3d_NOM_METALIZED
		QObject::tr("Neon GNC"),//("Неон GNC"),						// Graphic3d_NOM_NEON_GNC
		QObject::tr("Chrome"),//("Хром"),							// Graphic3d_NOM_CHROME
		QObject::tr("Aluminium"),//("Алюминий"),						// Graphic3d_NOM_ALUMINIUM
		QObject::tr("Obsidian"),//("Обсидиан"),						// Graphic3d_NOM_OBSIDIAN
		QObject::tr("Neon PHC"),//("Неон PHC"),						// Graphic3d_NOM_NEON_PHC
		QObject::tr("Jade"),//("Гейд"),						 	// Graphic3d_NOM_JADE
		QString()
	};

}

Model::Model(QObject* parent) : QObject(parent)
{
	if(defaultDevice.IsNull())
		defaultDevice = new Graphic3d_GraphicDevice(getenv("DISPLAY"));

	TCollection_ExtendedString v3DName("Visu3D");

	viewer = new V3d_Viewer(defaultDevice, v3DName.ToExtString(), "", 1000.0, V3d_XposYnegZpos,
						Quantity_NOC_GRAY30, V3d_ZBUFFER, V3d_GOURAUD, V3d_WAIT,
						Standard_True, Standard_True, V3d_TEX_NONE);

	viewer->Init();
	viewer->SetDefaultLights();
	viewer->SetLightOn();

	context = new AIS_InteractiveContext(viewer);

	//context->CurrentViewer()->CreateView();

	shapes = boost::shared_ptr<AIS_SequenceOfInteractive>(new AIS_SequenceOfInteractive());
}

void Model::loadModel(QString& fileName)
{
	Handle(TopTools_HSequenceOfShape) dsShapes = new TopTools_HSequenceOfShape();
	FileHelper::readFile(fileName, dsShapes);

	shapes->Clear();

	if (dsShapes.IsNull() || !dsShapes->Length())
		throw FileError(QObject::tr("Ошибка чтения элементов"));

	for (int i = 1; i <= dsShapes->Length(); ++i)
	{
		Handle(AIS_Shape) shape = new AIS_Shape(dsShapes->Value(i));
		context->SetDisplayMode(shape, 1, false);

		shapes->Append(shape);

		context->Display(shape, false);
	}

	context->UpdateCurrentViewer();

	this->fileName = fileName;

	Q_EMIT changed();
	Q_EMIT fileNameChanged(fileName);
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

Handle(AIS_InteractiveContext) Model::getContext() const
{
	return context;
}

boost::shared_ptr<AIS_SequenceOfInteractive> Model::getShapes() const
{
	return shapes;
}

boost::shared_ptr<AIS_SequenceOfInteractive> Model::getSelectedShapes() const
{
	boost::shared_ptr<AIS_SequenceOfInteractive> selected(new AIS_SequenceOfInteractive());

    for (context->InitCurrent(); context->MoreCurrent(); context->NextCurrent())
	{
		Handle(AIS_InteractiveObject) object = context->Current();

		if (object->IsKind(STANDARD_TYPE(AIS_Shape)))
			selected->Append(object);
	}

	return selected;
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
}

void Model::createRectangle(gp_Pnt& pt, float width, float height)
{
	gp_Pnt pt1(pt);
	gp_Pnt pt2(pt.X() + width, pt.Y(), pt.Z());
	gp_Pnt pt3(pt.X() + width, pt.Y() + height, pt.Z());
	gp_Pnt pt4(pt.X(), pt.Y() + height, pt.Z());

    Handle(Geom_TrimmedCurve) segment1 = GC_MakeSegment(pt1, pt2);
    Handle(Geom_TrimmedCurve) segment2 = GC_MakeSegment(pt2, pt3);
    Handle(Geom_TrimmedCurve) segment3 = GC_MakeSegment(pt3, pt4);
    Handle(Geom_TrimmedCurve) segment4 = GC_MakeSegment(pt4, pt1);

	TopoDS_Edge edge1 = BRepBuilderAPI_MakeEdge(segment1);
	TopoDS_Edge edge2 = BRepBuilderAPI_MakeEdge(segment2);
	TopoDS_Edge edge3 = BRepBuilderAPI_MakeEdge(segment3);
	TopoDS_Edge edge4 = BRepBuilderAPI_MakeEdge(segment4);

	TopoDS_Wire wire  = BRepBuilderAPI_MakeWire(edge1 , edge2 , edge3, edge4);

	TopoDS_Face face = BRepBuilderAPI_MakeFace(wire);

	/*gp_Vec prismVec(0, 0, 100);
	TopoDS_Shape body = BRepPrimAPI_MakePrism(face, prismVec);*/

	Handle(AIS_Shape) rect = new AIS_Shape(face);

	shapes->Append(rect);

	context->Display(rect, false);
	context->SetDisplayMode(rect, true, true);
}

void Model::createCircle(gp_Pnt& pt, float radius)
{
}

void Model::makePrism(Handle(AIS_Shape)& shape, float height)
{
	gp_Vec prismVec(0, 0, height);
	TopoDS_Shape body = BRepPrimAPI_MakePrism(shape->Shape(), prismVec);

	Handle(AIS_Shape) newShape = new AIS_Shape(body);

	//shapes->Erase(shape);
	context->Erase(shape);

	shapes->Append(newShape);

	context->Display(newShape, false);
	context->SetDisplayMode(newShape, true, true);
}

QString Model::getMaterialName(Graphic3d_NameOfMaterial material)
{
	return material_names[material];
}

Graphic3d_NameOfMaterial Model::getMaterialType(const QString& name)
{
	const QString* begin = material_names;
	const QString* end = material_names + MATERIALS_COUNT;
	const QString* res = std::find(begin, end, name);

	return static_cast<Graphic3d_NameOfMaterial>(res - begin);
}

QStringList Model::getMaterials()
{
	QStringList res;
	for (int i = 0; i < MATERIALS_COUNT - 1; ++i)
		res << material_names[i];

	return res;
}

