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
#include <AIS_SequenceOfInteractive.hxx>
#include <gp_Pnt.hxx>
#include <AIS_Shape.hxx>

#include <list>
#include <map>
#include <boost/shared_ptr.hpp>

#include <QStringList>

#include "error.h"

class AIS_Shape;

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
	boost::shared_ptr<AIS_SequenceOfInteractive> getShapes() const;
	boost::shared_ptr<AIS_SequenceOfInteractive> getSelectedShapes() const;

	static QString getMaterialName(Graphic3d_NameOfMaterial material);
	static Graphic3d_NameOfMaterial getMaterialType(const QString& name);
	static QStringList getMaterials();

	void makePrism(Handle(AIS_Shape)& shape, float height);

Q_SIGNALS:
	void changed();
	void fileNameChanged(QString& newFileName);

public Q_SLOTS:
	void loadModel(QString& fileName);
	void saveModel(QString& fileName);
	void setMaterial(Graphic3d_NameOfMaterial material);
	void setMaterial(const QString& material);
	void setShadded(bool shadded);
	void createRectangle(gp_Pnt& pt, float width, float height);
	void createCircle(gp_Pnt& pt, float radius);

private:
	QString fileName;

	Handle(V3d_Viewer) viewer;
	Handle(AIS_InteractiveContext) context;

	boost::shared_ptr<AIS_SequenceOfInteractive> shapes;
};

#endif // MODEL_HEADER

