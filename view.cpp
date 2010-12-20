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

#include <QColormap>
#include <QWheelEvent>
#include <QDebug>

#include <Xw_Window.hxx>
#include <Graphic3d_GraphicDevice.hxx>

#include <QX11Info>
#include <GL/glx.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#include <X11/Xmu/StdCmap.h>
#include <X11/Xlib.h>
#include <Xw_Window.hxx>
#include <Graphic3d_GraphicDevice.hxx>

View::View(QWidget* parent) : QGLWidget(parent),
	model(0), firstPaint(true), pressedX(0), pressedY(0), curAction(caNone), modKey(mkNone)
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

void View::updateView()
{
}

void View::init()
{
	view = model->getContext()->CurrentViewer()->CreateView();

	int windowHandle = (int)winId();
	short hi, lo;
	lo = (short) windowHandle;
	hi = (short) (windowHandle >> 16);
	Handle(Xw_Window) hWnd = new Xw_Window(Handle(Graphic3d_GraphicDevice)::
		DownCast(model->getContext()->CurrentViewer()->Device()),(int) hi,(int) lo,Xw_WQ_SAMEQUALITY);
	view->SetWindow(hWnd);
    if (!hWnd->IsMapped())
		hWnd->Map();
	view->SetBackgroundColor(Quantity_NOC_BLACK);
	view->MustBeResized();
}

void View::paintEvent(QPaintEvent*)
{
	if (firstPaint)
	{
		init();
		firstPaint = false;
	}

	view->Redraw();
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
			view->Pan(point.x() - pressedX, pressedY - point.y());
			break;
		}
		case caRotate:
		{
			view->Rotation(point.x(), point.y());
			view->Redraw();
			break;
		}
		default:
		{
			model->getContext()->MoveTo(point.x(), point.y(), view);
			break;
		}
	}

	pressedX = point.x();
	pressedY = point.y();
}

void View::mouseReleaseEvent(QMouseEvent*)
{
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
	pressedX = point.x();
	pressedY = point.y();

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
	}
}

void View::onRButtonDown(const int, const QPoint point)
{
	pressedX = point.x();
	pressedY = point.y();
}

void View::onMButtonDown(const int, const QPoint point)
{
	pressedX = point.x();
	pressedY = point.y();
}

