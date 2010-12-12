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

#include <QDebug>

#include <Graphic3d_GraphicDevice.hxx>
#include <AIS_Shape.hxx>

static Handle(Graphic3d_GraphicDevice) defaultDevice;

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
		context->Display(new AIS_Shape(shapes->Value(i)), false);

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

