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

#include "childwindow.h"
#include "view.h"
#include "controller.h"
#include "model.h"
//#include "qshapemodel.h"

#include <QCoreApplication>

#include <QDebug>
#include <QFocusEvent>

namespace Gui
{

	ChildWindow::ChildWindow(Model* model, QWidget* parent) : QMdiSubWindow(parent),
		model(model), view(0), controller(0), shapeModel(0)
	{
		controller = new Controller(this);

		Q_ASSERT(model);

		connect(model, SIGNAL(changed()),
			this, SLOT(modelChanged()));
		connect(model, SIGNAL(fileNameChanged(QString&)),
			this, SLOT(modelFileNameChanged(QString&)));

		createUI();
	}

	void ChildWindow::createUI()
	{
		view = new View(this);
		setWidget(view);
		connect(controller, SIGNAL(updateViewRequest()),
			view, SLOT(updateView()));

		view->setModel(model);

		connect(view, SIGNAL(selectionChanged()),
			this, SIGNAL(selectionChanged()));

		connect(controller, SIGNAL(loadModelRequest(QString&)),
			model, SLOT(loadModel(QString&)));

		//shapeModel = new QShapeModel(model, this);

		connect(controller, SIGNAL(setMaterialRequest(Graphic3d_NameOfMaterial)),
			model, SLOT(setMaterial(Graphic3d_NameOfMaterial)));
		connect(controller, SIGNAL(setShaddedRequest(bool)), model, SLOT(setShadded(bool)));
		connect(controller, SIGNAL(createRectangleRequest(gp_Pnt&, float, float)),
			model, SLOT(createRectangle(gp_Pnt&, float, float)));
		connect(controller, SIGNAL(makePrismRequest(AIS_Shape*, float)),
			model, SLOT(makePrism(AIS_Shape*, float)));
	}

	Controller* ChildWindow::getController() const
	{
		return controller;
	}

	View* ChildWindow::getView() const
	{
		return view;
	}

	QShapeModel* ChildWindow::getShapeModel() const
	{
		return shapeModel;
	}

	void ChildWindow::modelChanged()
	{
	}

	void ChildWindow::modelFileNameChanged(QString& fileName)
	{
		setWindowTitle(fileName);
	}

	void ChildWindow::keyPressEvent(QKeyEvent* event)
	{
		//QCoreApplication::sendEvent(view, event);
	}

	void ChildWindow::keyReleaseEvent(QKeyEvent* event)
	{
		//QCoreApplication::sendEvent(view, event);
	}

}

