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
#include "cadapplication.h"

#include <stdlib.h>
#include <algorithm>

#include <QDebug>
#include <QApplication>

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
#include <AIS_Plane.hxx>
#include <TCollection_ExtendedString.hxx>
#include <BRepPrimAPI_MakeCylinder.hxx>
#include <BRepPrimAPI_MakeCone.hxx>
#include <BRepPrimAPI_MakeBox.hxx>
#include <BRepPrimAPI_MakeTorus.hxx>
#include <BRepPrimAPI_MakeSphere.hxx>
#include <BRepAlgoAPI_Fuse.hxx>
#include <BRepAlgoAPI_Common.hxx>
#include <BRepAlgoAPI_Cut.hxx>
#include <BRepBuilderAPI_GTransform.hxx>
#include <Geom_Plane.hxx>
#include <GC_MakePlane.hxx>
#include <gp_Pln.hxx>
#include <gp_GTrsf.hxx>

#ifdef Q_OS_LINUX
	#include <Graphic3d_GraphicDevice.hxx>
#elif defined Q_OS_WIN32 
	#include <Graphic3d_WNTGraphicDevice.hxx>
#endif

#include <Inventor/Qt/viewers/SoQtPlaneViewer.h>
#include <Inventor/nodes/SoSeparator.h>
#include "viewprovider.h"
#include "inventorviewer.h"

namespace
{

#ifdef Q_OS_LINUX
	Handle(Graphic3d_GraphicDevice) defaultDevice;
#elif defined Q_OS_WIN32 
	Handle(Graphic3d_WNTGraphicDevice) defaultDevice;
#endif

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
	viewer = new InventorViewer();
	separator = new SoSeparator();
	separator->ref();
	viewer->setSceneGraph(separator);
}

Model::~Model()
{
	separator->unref();
	delete viewer;
}

void Model::loadModel(QString& fileName)
{
	Handle(TopTools_HSequenceOfShape) dsShapes = new TopTools_HSequenceOfShape();
	FileHelper::readFile(fileName, dsShapes);


	if (dsShapes.IsNull() || !dsShapes->Length())
		throw FileError(QObject::tr("Ошибка чтения элементов"));

	ViewProvider provider;

	for (int i = 1; i <= dsShapes->Length(); ++i)
	{
		/*Handle(AIS_Shape) shape = new AIS_Shape(dsShapes->Value(i));
		context->SetDisplayMode(shape, 1, false);

		shapes->Append(shape);

		context->Display(shape, false);*/

		SoGroup* faces = new SoGroup();
		faces->ref();
		/*SoGroup* vert = new SoGroup();
		vert->ref();
		SoGroup* edges = new SoGroup();
		edges->ref();*/

		provider.computeFaces(faces, dsShapes->Value(i), .2);
		/*computeVertices(vert, dsShapes->Value(i));
		computeEdges(edges, dsShapes->Value(i));*/

		separator->addChild(faces);
		/*separator->addChild(vert);
		separator->addChild(edges);*/
	}

	//context->UpdateCurrentViewer();

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

Handle(AIS_InteractiveContext) Model::getContext() const
{
	return context;
}

/*boost::shared_ptr<AIS_SequenceOfInteractive> Model::getShapes() const
{
	return shapes;
}

boost::shared_ptr<AIS_SequenceOfInteractive> Model::getSelectedShapes() const
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

	Handle(AIS_Shape) rect = new AIS_Shape(face);

	//shapes->Append(rect);

	context->Display(rect, false);
	context->SetDisplayMode(rect, true, true);
}

void Model::createCircle(gp_Pnt& pt, float radius)
{
}

void Model::createPlane(const gp_Ax3& axis, float height, float width)
{
	Handle(Geom_Plane) plane = new Geom_Plane(axis);
	TopoDS_Face face = BRepBuilderAPI_MakeFace(plane, 0.0, height, 0.0, width);

	Handle(AIS_Shape) shape = new AIS_Shape(face);
	//shapes->Append(shape);
	context->Display(shape, false);
	context->SetDisplayMode(shape, true, true);
}

void Model::createBox(const gp_Ax3& axis, float height, float width,
	float length)
{
	gp_Ax2 ax2(axis.Location(), axis.Direction());
	BRepPrimAPI_MakeBox makeBox(ax2, height, width, length);
	Handle(AIS_Shape) shape = new AIS_Shape(makeBox.Shape());

	//shapes->Append(shape);
	context->Display(shape, false);
	context->SetDisplayMode(shape, true, true);
}

void Model::createCylinder(const gp_Ax3& axis, float radius, float height,
	float angle)
{
	gp_Ax2 ax2(axis.Location(), axis.Direction());
	BRepPrimAPI_MakeCylinder makeCylinder(ax2, radius, height);
	Handle(AIS_Shape) shape = new AIS_Shape(makeCylinder.Shape());

	//shapes->Append(shape);
	context->Display(shape, false);
	context->SetDisplayMode(shape, true, true);
}

void Model::createCone(const gp_Ax3& axis, float radius1, float radius2,
	float height, float angle)
{
	gp_Ax2 ax2(axis.Location(), axis.Direction());
	BRepPrimAPI_MakeCone makeCone(ax2, radius1, radius2, height, angle/180.0f*Standard_PI);
	Handle(AIS_Shape) shape = new AIS_Shape(makeCone.Shape());

	//shapes->Append(shape);
	context->Display(shape, false);
	context->SetDisplayMode(shape, true, true);
}

void Model::createSphere(const gp_Ax3& axis, float radius, float angle)
{
	gp_Ax2 ax2(axis.Location(), axis.Direction());
	BRepPrimAPI_MakeSphere makeSphere(ax2, radius, angle/180.0f*Standard_PI);
	Handle(AIS_Shape) shape = new AIS_Shape(makeSphere.Shape());

	//shapes->Append(shape);
	context->Display(shape, false);
	context->SetDisplayMode(shape, true, true);
}

void Model::createEllipsoid(const gp_Ax3& axis, float radius1, float radius2,
	float angle, float uParam, float vParam)
{
    gp_Pnt pnt(axis.Location());
    gp_Dir dir(axis.Direction());
    gp_Ax2 ax2(pnt, dir);

	BRepPrimAPI_MakeSphere makeSphere(ax2, radius2, angle/180.0f*Standard_PI);

	float scale = radius1 / radius2;
	gp_Dir xDir = ax2.XDirection();
	gp_Dir yDir = ax2.YDirection();
	gp_GTrsf mat;
	mat.SetValue(1, 1, xDir.X());
	mat.SetValue(2, 1, xDir.Y());
	mat.SetValue(3, 1, xDir.Z());
	mat.SetValue(1, 2, yDir.X());
	mat.SetValue(2, 2, yDir.Y());
	mat.SetValue(3, 2, yDir.Z());
	mat.SetValue(1, 3, dir.X()*scale);
	mat.SetValue(2, 3, dir.Y()*scale);
	mat.SetValue(3, 3, dir.Z()*scale);
	BRepBuilderAPI_GTransform makeTrsf(makeSphere.Shape(), mat);

	Handle(AIS_Shape) shape = new AIS_Shape(makeTrsf.Shape());
	//shapes->Append(shape);
	context->Display(shape, false);
	context->SetDisplayMode(shape, true, true);
}

void Model::createTorus(const gp_Ax3& axis, float radius1, float radius2,
	float angle, float uParam, float vParam)
{
	gp_Ax2 ax2(axis.Location(), axis.Direction());
	BRepPrimAPI_MakeTorus makeTorus(ax2, radius1, radius2, angle/180.0f*Standard_PI);

	Handle(AIS_Shape) shape = new AIS_Shape(makeTorus.Shape());
	//shapes->Append(shape);
	context->Display(shape, false);
	context->SetDisplayMode(shape, true, true);
}

void Model::showDatumPlane()
{
	context->CurrentViewer()->ActivateGrid(Aspect_GT_Rectangular, Aspect_GDM_Lines);
	context->CurrentViewer()->SetGridEcho(Standard_True);
}

void Model::hideDatumPlane()
{
	context->CurrentViewer()->DeactivateGrid();
	context->CurrentViewer()->SetGridEcho(Standard_False);
}

void Model::setDatumPlaneXY()
{
	gp_Ax3 plane(gp_Pnt(0.0, 0.0, 0.0), gp_Dir(0.0, 0.0, 1.0));
	context->CurrentViewer()->SetPrivilegedPlane(plane);
}

void Model::setDatumPlaneXZ()
{
	gp_Ax3 plane(gp_Pnt(0.0, 0.0, 0.0), gp_Dir(0.0, -1.0, 0.0));
	context->CurrentViewer()->SetPrivilegedPlane(plane);
}

void Model::setDatumPlaneYZ()
{
	gp_Ax3 plane(gp_Pnt(0.0, 0.0, 0.0), gp_Dir(1.0, 0.0, 0.0));
	context->CurrentViewer()->SetPrivilegedPlane(plane);
}

void Model::setDatumPlane(const gp_Pnt& pt, const gp_Dir& dir)
{
	gp_Ax3 plane(pt, dir);
	context->CurrentViewer()->SetPrivilegedPlane(plane);
}

void Model::getDatumPlane(gp_Pnt& pt, gp_Dir& dir) const
{
	gp_Ax3 axis = context->CurrentViewer()->PrivilegedPlane();
	pt = axis.Location();
	dir = axis.Direction();
}

void Model::makePrism(const Handle(AIS_Shape)& shape, float height)
{
	gp_Vec prismVec(0, 0, height);
	TopoDS_Shape body = BRepPrimAPI_MakePrism(shape->Shape(), prismVec);

	Handle(AIS_Shape) newShape = new AIS_Shape(body);

	//shapes->Erase(shape);
	context->Erase(shape);

	//shapes->Append(newShape);

	context->Display(newShape, false);
	context->SetDisplayMode(newShape, true, true);
}

void Model::fuse(const Handle(AIS_Shape)& shape1, const Handle(AIS_Shape)& shape2)
{
	TopoDS_Shape shape = BRepAlgoAPI_Fuse(shape1->Shape(), shape2->Shape());

	removeShape(shape1);
	removeShape(shape2);

	Handle(AIS_Shape) newShape = new AIS_Shape(shape);

	//shapes->Append(newShape);

	context->Display(newShape, false);
	context->SetDisplayMode(newShape, true, true);
}

void Model::common(const Handle(AIS_Shape)& shape1, const Handle(AIS_Shape)& shape2)
{
	TopoDS_Shape shape = BRepAlgoAPI_Common(shape1->Shape(), shape2->Shape());

	removeShape(shape1);
	removeShape(shape2);

	Handle(AIS_Shape) newShape = new AIS_Shape(shape);

	//shapes->Append(newShape);

	context->Display(newShape, false);
	context->SetDisplayMode(newShape, true, true);
}

void Model::cut(const Handle(AIS_Shape)& shape1, const Handle(AIS_Shape)& shape2)
{
	TopoDS_Shape shape = BRepAlgoAPI_Cut(shape1->Shape(), shape2->Shape());

	removeShape(shape1);
	removeShape(shape2);

	Handle(AIS_Shape) newShape = new AIS_Shape(shape);

	//shapes->Append(newShape);

	context->Display(newShape, false);
	context->SetDisplayMode(newShape, true, true);
}

void Model::selectNeutral()
{
	context->CloseAllContexts();
}

void Model::selectVertex()
{
	context->CloseAllContexts();
	context->OpenLocalContext();
	context->ActivateStandardMode(TopAbs_VERTEX);
}

void Model::selectEdge()
{
	context->CloseAllContexts();
	context->OpenLocalContext();
	context->ActivateStandardMode(TopAbs_EDGE);
}

void Model::selectFace()
{
	context->CloseAllContexts();
	context->OpenLocalContext();
	context->ActivateStandardMode(TopAbs_FACE);
}

void Model::selectSolid()
{
	context->CloseAllContexts();
	context->OpenLocalContext();
	context->ActivateStandardMode(TopAbs_SOLID);
}

void Model::removeShape(const Handle(AIS_InteractiveObject)& shape)
{
	// TODO: Удаление шейпа из коллекции
	context->Erase(shape);
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

