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

	shapes = new TopTools_HSequenceOfShape();
}

void Model::loadModel(QString& fileName) throw(FileError)
{
	FileHelper::readFile(fileName, shapes);

	if (shapes.IsNull() || !shapes->Length())
		throw FileError(QObject::tr("Ошибка чтения элементов"));

	for (int i = 1; i <= shapes->Length(); ++i)
	{
		Handle(AIS_Shape) shape = new AIS_Shape(shapes->Value(i));
		context->SetDisplayMode(shape, 0, false);

		context->Display(shape, false);
	}

	context->UpdateCurrentViewer();

	this->fileName = fileName;

	Q_EMIT changed();
	Q_EMIT fileNameChanged(fileName);
}

void Model::saveModel(QString& fileName) throw(FileError)
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

Handle(TopTools_HSequenceOfShape) Model::getShapes() const
{
	return shapes;
}

Handle(TopTools_HSequenceOfShape) Model::getSelectedShapes() const
{
	Handle(TopTools_HSequenceOfShape) selected = new TopTools_HSequenceOfShape();

    for (context->InitCurrent(); context->MoreCurrent(); context->NextCurrent())
	{
		Handle(AIS_InteractiveObject) object = context->Current();

		if (object->IsKind(STANDARD_TYPE(AIS_Shape)))
		{
			TopoDS_Shape shape = Handle(AIS_Shape)::DownCast(object)->Shape();
			selected->Append(shape);
		}
	}

	return selected;
}

void Model::setMaterial(Graphic3d_NameOfMaterial material)
{
    for (context->InitCurrent(); context->MoreCurrent(); context->NextCurrent())
	{
		Handle(AIS_InteractiveObject) object = context->Current();

		shapeMaterialMap[object] = material;

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

Graphic3d_NameOfMaterial Model::getShapeMaterial(const Handle(AIS_Shape)& shape) const
{
	std::map<Handle(AIS_InteractiveObject), Graphic3d_NameOfMaterial>::const_iterator
		it = shapeMaterialMap.find(shape);

	if (it == shapeMaterialMap.end())
		return Graphic3d_NOM_DEFAULT;

	return (*it).second;
}

