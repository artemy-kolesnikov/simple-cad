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

#include "view.h"
#include "model.h"
#include "cadapplication.h"

#include <QColormap>
#include <QWheelEvent>
#include <QRubberBand>
#include <QX11Info>
#include <QDebug>
#include <QList>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glx.h>

#include <algorithm>

#include <X11/Xutil.h>
#include <X11/Xatom.h>
#include <X11/Xmu/StdCmap.h>
#include <X11/Xlib.h>
#include <Xw_Window.hxx>
#include <Graphic3d_GraphicDevice.hxx>
#include <TopExp_Explorer.hxx> 
#include <TopoDS_Face.hxx> 
#include <TopoDS.hxx>
#include <Geom_Surface.hxx>
#include <BRep_Tool.hxx>
#include <Geom_Plane.hxx>
#include <gp_Ax3.hxx>
#include <Geom_Axis2Placement.hxx>
#include <AIS_Trihedron.hxx>

View::View(QWidget* parent) : QWidget(parent),
	model(0), firstPaint(true), rectBand(0), curAction(caNone),
	modKey(mkNone)
{
	createUI();
}

void View::createUI()
{
	XSynchronize(x11Info().display(), true);

	setAttribute(Qt::WA_PaintOnScreen);
	setAttribute(Qt::WA_NoSystemBackground);

	setMouseTracking(true);
}

void View::setModel(Model* model)
{
	this->model = model;

	connect(model, SIGNAL(changed()),
		this, SLOT(updateView()));
}

Model* View::getModel() const
{
	return model;
}

void View::viewFront()
{
	view->SetProj(V3d_Yneg);
}

void View::viewBack()
{
	view->SetProj(V3d_Ypos);
}

void View::viewTop()
{
	view->SetProj(V3d_Zpos);
}

void View::viewBottom()
{
	view->SetProj(V3d_Zneg);
}

void View::viewLeft()
{
	view->SetProj(V3d_Xneg);
}

void View::viewRight()
{
	view->SetProj(V3d_Xpos);
}

void View::viewDatumPlane()
{
	view->SetFront();
}

void View::init()
{
	view = model->getContext()->CurrentViewer()->CreateView();

	int windowHandle = (int)winId();
	short hi, lo;
	lo = (short) windowHandle;
	hi = (short) (windowHandle >> 16);
	Handle(Xw_Window) hWnd = new Xw_Window(Handle(Graphic3d_GraphicDevice)::
		DownCast(model->getContext()->CurrentViewer()->Device()),(int) hi,(int) lo, Xw_WQ_SAMEQUALITY);
	view->SetWindow(hWnd, 0, paintCallBack, this);

    if (!hWnd->IsMapped())
		hWnd->Map();
	view->SetBackgroundColor(Quantity_NOC_BLACK);
	view->MustBeResized();

	/*Handle(Geom_Axis2Placement) trihedronAxis = new Geom_Axis2Placement(gp::XOY());
	Handle(AIS_Trihedron) trihedron = new AIS_Trihedron(trihedronAxis);
	model->getContext()->Display(trihedron);*/

	//view->TriedronDisplay(Aspect_TOTP_LEFT_LOWER, Quantity_NOC_WHITE, 0.1, V3d_ZBUFFER);
}

void View::paintEvent(QPaintEvent*)
{
	if (firstPaint)
	{
		init();
		firstPaint = false;
	}

	if( !view.IsNull() )
	{
		view->Redraw();
	}
}

void View::resizeEvent(QResizeEvent*)
{
	if( !view.IsNull() )
	{
		view->MustBeResized();
	}
}

void View::wheelEvent(QWheelEvent* event)
{
	if (event->delta() < 0)
		view->SetScale(1.05);
	else
		view->SetScale(0.95);

	view->SetViewMappingDefault();
}

void View::mousePressEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton)
		onLButtonDown((event->buttons() | event->modifiers()), event->pos());
	else if (event->button() == Qt::MidButton)
		onMButtonDown(event->buttons() | event->modifiers(), event->pos());
	else if (event->button() == Qt::RightButton)
		onRButtonDown(event->buttons() | event->modifiers(), event->pos() );
}

void View::mouseMoveEvent(QMouseEvent* event)
{
	QPoint point = event->pos();

	switch (curAction)
	{
		case caMove:
		{
			view->Pan(point.x() - panPoint.x(), panPoint.y() - point.y());
			break;
		}
		case caRotate:
		{
			view->Rotation(point.x(), point.y());
			view->Redraw();
			break;
		}
		case caRectSelect:
		{
			rectBand->setGeometry(QRect(pressedPoint, point).normalized());
			model->getContext()->MoveTo(point.x(), point.y(), view);
			model->getContext()->Select(pressedPoint.x(), pressedPoint.y(), point.x(), point.y(), view);

			Q_EMIT selectionChanged();

			break;
		}
		default:
		{
			model->getContext()->MoveTo(point.x(), point.y(), view);
			break;
		}
	}

	panPoint = point;
}

void View::mouseReleaseEvent(QMouseEvent*)
{
	if (caRectSelect == curAction)
	{
		Q_ASSERT(rectBand);
		rectBand->hide();
	}

	curAction = caNone;
}

void View::keyPressEvent(QKeyEvent* event)
{
	if (event->key() == Qt::Key_Space)
	{
		modKey = mkSpace;
	}
}

void View::keyReleaseEvent(QKeyEvent* event)
{
	if (event->key() == Qt::Key_Space)
		modKey = mkNone;
}

void View::onLButtonDown(const int flags, const QPoint point)
{
	pressedPoint = point;

	if (flags & Qt::ControlModifier)
		curAction = caMove;
	else if (modKey == mkSpace)
	{
		curAction = caRotate;
		view->StartRotation(point.x(), point.y());
	}
	else if (flags & Qt::ShiftModifier)
	{
		model->getContext()->ShiftSelect();

		Q_EMIT selectionChanged();
	}
	else
	{
		model->getContext()->Select();

		Q_EMIT selectionChanged();

		selectHook();

		curAction = caRectSelect;

		if (!rectBand)
			rectBand = new QRubberBand(QRubberBand::Rectangle, this);
		rectBand->setGeometry(QRect(pressedPoint, QSize()));
		rectBand->show();
	}
}

void View::onRButtonDown(const int, const QPoint point)
{
	pressedPoint = point;
}

void View::onMButtonDown(const int, const QPoint point)
{
	pressedPoint = point;
}

void View::paintGl()
{
	glDisable(GL_LIGHTING); 
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
    glMatrixMode(GL_PROJECTION);
	glPushMatrix();
    glLoadIdentity();

	GLfloat left   = -1.0f;
	GLfloat right  =  1.0f;
	GLfloat bottom = -1.0f;
	GLfloat top    =  1.0f;
	GLfloat depth  =  1.0f;

    glOrtho(left, right, bottom, top, 1.0, -1.0);

//#ifndef OCC_PATCHED
	/*glEnable(GL_BLEND);
	if (view->ColorScaleIsDisplayed())
	{
		// Not needed on patched OCC 6.2 versions, but is the lowest
		// common denominator working code on collaborators OpenGL
		// graphics cards.
		glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_COLOR);
	}*/
//#endif

    glBegin( GL_QUADS);
    {
      glColor4f(0.1f, 0.1f, 0.1f, 1.0f);
      glVertex3d(left, bottom, depth);
      glVertex3d(right, bottom, depth);
      glColor4f(0.8f, 0.8f, 0.9f, 1.0f);
      glVertex3d(right, top, depth);
      glVertex3d(left, top, depth);
    }
    glEnd();

	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

int View::paintCallBack(Aspect_Drawable drawable, void* userData,
	Aspect_GraphicCallbackStruct* data)
{
	View* view = static_cast<View*>(userData);
	view->paintGl();

	return 0;
}

void View::selectHook()
{
	/*boost::shared_ptr<AIS_SequenceOfInteractive> shapes = model->getSelectedShapes();
	if (shapes->Length() == 1)
	{
		TopoDS_Shape shape = Handle(AIS_Shape)::DownCast(shapes->Value(1))->Shape();

		for(TopExp_Explorer faceExplorer(shape, TopAbs_FACE); faceExplorer.More(); faceExplorer.Next())
		{
			TopoDS_Face face = TopoDS::Face(faceExplorer.Current());

			Handle(Geom_Surface) surface = BRep_Tool::Surface(face);

			if(surface->DynamicType() == STANDARD_TYPE(Geom_Plane))
			{
				Handle(Geom_Plane) plane = Handle(Geom_Plane)::DownCast(surface);
				gp_Ax3 pos = plane->Position();
				gp_Dir dir = pos.Direction();
				gp_Vec vec(dir);

				//gp_Vec v(gp_Pnt(2, 34, 54), gp_Pnt(45, 65, 22));
				gp_Vec v(gp_Pnt(0, 0, 0), gp_Pnt(1, 0, 0));
				gp_Dir d(v);

				model->getContext()->CurrentViewer()->SetPrivilegedPlane(pos);
			}
		}
	}*/
}

