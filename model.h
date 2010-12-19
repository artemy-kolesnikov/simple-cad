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

#ifndef MODEL_HEADER
#define MODEL_HEADER

#include <QObject>
#include <AIS_InteractiveContext.hxx>
#include <V3d_Viewer.hxx>
#include <TopTools_HSequenceOfShape.hxx>
#include <AIS_Shape.hxx>

#include <list>
#include <map>

#include <QStringList>

#include "error.h"

/**
 * Keeps loaded CAD model
 */
class Model : public QObject
{
	Q_OBJECT

public:
	Model(QObject* parent = 0);

	QString getFileName() const;

	Handle(AIS_InteractiveContext) getContext() const;
	Handle(TopTools_HSequenceOfShape) getShapes() const;

	static QString getMaterialName(Graphic3d_NameOfMaterial material);
	static Graphic3d_NameOfMaterial getMaterialType(const QString& name);
	static QStringList getMaterials();

	Graphic3d_NameOfMaterial getShapeMaterial(const Handle(AIS_InteractiveObject)& shape) const;

Q_SIGNALS:
	void changed();
	void fileNameChanged(QString& newFileName);

public Q_SLOTS:
	void loadModel(QString& fileName) throw(FileError);
	void saveModel(QString& fileName) throw(FileError);
	void setMaterial(Graphic3d_NameOfMaterial material);
	void setMaterial(const QString& material);
	void setShadded(bool shadded);

private:
	QString fileName;

	Handle(V3d_Viewer) viewer;
	Handle(AIS_InteractiveContext) context;

	Handle(TopTools_HSequenceOfShape) shapes;

	// TODO: Perhaps there is a built-in AIS_Shape list
	std::list<Handle(AIS_Shape)> ais_shapes;

	// TODO: Perhaps there is a built-in way to get a shape material name
	std::map<Handle(AIS_InteractiveObject), Graphic3d_NameOfMaterial> shapeMaterialMap;
};

#endif // MODEL_HEADER

