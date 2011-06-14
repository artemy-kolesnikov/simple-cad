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

#include "filletcommand.h"

#include <filletoperationwidget.h>
#include <cadapplication.h>
#include <mainwindow.h>
#include <model.h>
#include <widgetdialog.h>

namespace Gui
{

	FilletCommand::FilletCommand(Model& model, const ViewerShape& shape) :
		model(model), shape(shape)
	{
	}

	void FilletCommand::execute()
	{
		MainWindow* win = CADApplication::getMainWindow();

		FilletOperationWidget* operationWidget = new FilletOperationWidget(model.getShapes(), win);
		WidgetDialog* dlg = new WidgetDialog(win);
		dlg->setCentralWidget(operationWidget);
		if (dlg->exec() == QDialog::Accepted)
		{
			std::auto_ptr<Action::CombineAction> action;
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
		}
	}

	QString FilletCommand::getName() const
	{
		return QObject::tr("Скругление");
	}

}

