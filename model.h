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
//#include <AIS_InteractiveContext.hxx>
#include <AIS_SequenceOfInteractive.hxx>
#include <gp_Pnt.hxx>
#include <AIS_Shape.hxx>
#include <V3d_Viewer.hxx>

#include <list>
#include <map>
#include <boost/shared_ptr.hpp>

#include <QStringList>

#include "error.h"

class AIS_Shape;
class SoQtViewer;
class SoSeparator;
class InventorViewer;

/**
 * Keeps loaded CAD model
 */
class Model : public QObject
{
	Q_OBJECT

public:
	Model(QObject* parent = 0);
	~Model();

	QString getFileName() const;

	Handle(AIS_InteractiveContext) getContext() const;
	/*boost::shared_ptr<AIS_SequenceOfInteractive> getShapes() const;
	boost::shared_ptr<AIS_SequenceOfInteractive> getSelectedShapes() const;
	boost::shared_ptr<AIS_SequenceOfInteractive> getCurrentShapes() const;*/

	static QString getMaterialName(Graphic3d_NameOfMaterial material);
	static Graphic3d_NameOfMaterial getMaterialType(const QString& name);
	static QStringList getMaterials();

	void makePrism(const Handle(AIS_Shape)& shape, float height);
	void fuse(const Handle(AIS_Shape)& shape1, const Handle(AIS_Shape)& shape2);
	void common(const Handle(AIS_Shape)& shape1, const Handle(AIS_Shape)& shape2);
	void cut(const Handle(AIS_Shape)& shape1, const Handle(AIS_Shape)& shape2);

	void selectNeutral();
	void selectVertex();
	void selectEdge();
	void selectFace();
	void selectSolid();

	void removeShape(const Handle(AIS_InteractiveObject)& shape);

	InventorViewer* getViewer() const { return viewer; }

Q_SIGNALS:
	void changed();
	void fileNameChanged(QString& newFileName);

public Q_SLOTS:
	void loadModel(QString& fileName);
	void saveModel(QString& fileName);
	/*void setMaterial(Graphic3d_NameOfMaterial material);
	void setMaterial(const QString& material);
	void setShadded(bool shadded);*/
	void createRectangle(gp_Pnt& pt, float width, float height);
	void createCircle(gp_Pnt& pt, float radius);

	void createPlane(const gp_Ax3& axis, float height, float width);
	void createBox(const gp_Ax3& axis, float height, float width, float length);
	void createCylinder(const gp_Ax3& axis, float radius, float height, float angle);
	void createCone(const gp_Ax3& axis, float radius1, float radius2, float height, float angle);
	void createSphere(const gp_Ax3& axis, float radius, float angle);
	void createEllipsoid(const gp_Ax3& axis, float radius1, float radius2, float angle, float uParam, float vParam);
	void createTorus(const gp_Ax3& axis, float radius1, float radius2, float angle, float uParam, float vParam);

	void showDatumPlane();
	void hideDatumPlane();
	void setDatumPlaneXY();
	void setDatumPlaneXZ();
	void setDatumPlaneYZ();
	void setDatumPlane(const gp_Pnt& pt, const gp_Dir& dir);
	void getDatumPlane(gp_Pnt& pt, gp_Dir& dir) const;

private:
	QString fileName;

	Handle(AIS_InteractiveContext) context;

	InventorViewer* viewer;
	SoSeparator* separator;
};

#endif // MODEL_HEADER

