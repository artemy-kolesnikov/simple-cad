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

#include "createprimitivecommand.h"

#include <cadapplication.h>
#include <create3dprimitive.h>
#include <createprimitivewidget.h>
#include <mainwindow.h>
#include <model.h>
#include <widgetdialog.h>

namespace Gui
{

	void CreatePrimitiveCommand::execute()
	{
		MainWindow* win = CADApplication::getMainWindow();

		CreatePrimitiveWidget* settingsWidget = new CreatePrimitiveWidget(win);
		settingsWidget->setType(type);
		WidgetDialog* dlg = new WidgetDialog(win);
		dlg->setCentralWidget(settingsWidget);
		if (dlg->exec() == QDialog::Accepted)
		{
			std::auto_ptr<Action::Create3dPrimitive> createAction;
			settingsWidget->getCreationAction(createAction);
			createAction->execute();
			model.addShape(createAction->getShape());
		}
	}

	QStringList CreatePrimitiveCommand::getObjectNames()
	{
		QStringList objectNames;
		objectNames 
			<< QObject::tr("Прямоугольник")
			<< QObject::tr("Окружность")
			<< QObject::tr("Плоскость")
			<< QObject::tr("Брусок")
			<< QObject::tr("Цилиндр")
			<< QObject::tr("Конус")
			<< QObject::tr("Сфера")
			<< QObject::tr("Еллипс")
			<< QObject::tr("Тор");

		return objectNames;
	}

}

