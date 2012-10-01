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

		connect(model, SIGNAL(fileNameChanged(QString&)),
			this, SLOT(modelFileNameChanged(QString&)));

		createUI();
	}

	void ChildWindow::createUI()
	{
		view = new View(this);
		setWidget(view);

		view->setModel(model);

		connect(view, SIGNAL(selectionChanged()),
			this, SIGNAL(selectionChanged()));

		//shapeModel = new QShapeModel(model, this);
	}

	Controller* ChildWindow::getController() const
	{
		return controller;
	}

	View& ChildWindow::getView() const
	{
		return *view;
	}

	QShapeModel* ChildWindow::getShapeModel() const
	{
		return shapeModel;
	}

	void ChildWindow::modelFileNameChanged(QString& fileName)
	{
		setWindowTitle(fileName);
	}

	void ChildWindow::closeEvent(QCloseEvent* event)
	{
		qDebug() << "closeEvent()";
		Q_EMIT closed();
		QWidget::closeEvent(event);
	}

}

