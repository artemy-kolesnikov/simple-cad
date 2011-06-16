/*
 * Simple CAD System
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

#include "boolcommand.h"

#include <booloperationwidget.h>
#include <cadapplication.h>
#include <mainwindow.h>
#include <model.h>
#include <view.h>
#include <widgetdialog.h>
#include <exception.h>
#include <pickshapewidget.h>

namespace Gui
{

	BooleanCommand::BooleanCommand(Model& model, View& view, OperationType type) :
		model(model), view(view), type(type)
	{
		setType(Command::Delayed);
	}

	BooleanCommand::~BooleanCommand()
	{
		operationWidget->deleteLater();
		pickWidget->deleteLater();
	}
	
	void BooleanCommand::doPrepare()
	{
		MainWindow* win = CADApplication::getMainWindow();

		if (model.getShapes().size() == 0)
			throw Common::Exception(QObject::tr("Отсутствуют объекты"));

		operationWidget = new BoolOperationWidget(model.getShapes(), type, win);
		pickWidget = new PickShapeWidget(view, win);
		pickWidget->setCentralWidget(operationWidget);
		win->setCommandWidget(pickWidget, MainWindow::CommandMod);
		QObject::connect(pickWidget, SIGNAL(shapeSelected(const ViewerShape&, const TopoDS_Shape&)),
			operationWidget, SLOT(shapeSelected(const ViewerShape&, const TopoDS_Shape&)));

		QObject::connect(pickWidget, SIGNAL(canceled()), this, SLOT(emitCanceled()));
		QObject::connect(pickWidget, SIGNAL(confirmed()), this, SLOT(emitReadyToExecute()));
	}

	void BooleanCommand::doExecute()
	{
		std::auto_ptr<Action::Boolean> action;
		operationWidget->getAction(action);
		action->execute();

		std::list<TopoDS_Shape> removedShapes(action->getRemovedShapes());
		std::list<TopoDS_Shape>::iterator it = removedShapes.begin();
		for (;it != removedShapes.end(); ++it)
		{
			model.removeShape(Shape(*it));
		}

		TopoDS_Shape topoShape = action->getShape();
		model.addShape(Shape(topoShape, getName()));

		MainWindow* win = CADApplication::getMainWindow();
		win->removeCommandWidget();
	}

	void BooleanCommand::doCancel()
	{
		MainWindow* win = CADApplication::getMainWindow();
		win->removeCommandWidget();
	}

	QString BooleanCommand::getName() const
	{
		return getOperationNameByType(type);
	}

	QString BooleanCommand::getOperationNameByType(OperationType type)
	{
		if (type == Fuse)
			return QObject::tr("Объединение");
		else if (type == Common)
			return QObject::tr("Пересечение");
		else if (type == Cut)
			return QObject::tr("Вычитание");
	}

}

