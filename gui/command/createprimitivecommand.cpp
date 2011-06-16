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
#include <shape.h>
#include <TopoDS_Shape.hxx>

namespace
{
	const int TYPE_COUNT = 9;
}

namespace Gui
{

	void CreatePrimitiveCommand::doExecute()
	{
		MainWindow* win = CADApplication::getMainWindow();

		CreatePrimitiveWidget* settingsWidget = new CreatePrimitiveWidget(win);
		settingsWidget->setType(type);
		WidgetDialog* dlg = new WidgetDialog(win);
		dlg->setCentralWidget(settingsWidget);
		if (dlg->exec() == QDialog::Accepted)
		{
			std::auto_ptr<Action::Create3dPrimitive> createAction;
			settingsWidget->getAction(createAction);
			createAction->execute();
			TopoDS_Shape topoShape = createAction->getShape();
			QString objectName = getObjectNameByType(settingsWidget->getType());
			model.addShape(Shape(topoShape, objectName));
		}
	}

	QString CreatePrimitiveCommand::getName() const
	{
		return QObject::tr("Создать примитив");
	}

	QStringList CreatePrimitiveCommand::getObjectNames()
	{
		QStringList objectNames;
		for (int i = 0; i < TYPE_COUNT; ++i)
			objectNames << getObjectNameByType(static_cast<ObjectType>(i));

		return objectNames;
	}

	QString CreatePrimitiveCommand::getObjectNameByType(CreatePrimitiveCommand::ObjectType type)
	{
		static QString objectTypeNames[TYPE_COUNT];
		static bool init = false;

		if (!init)
		{
			init = true;

			objectTypeNames[0] = QObject::tr("Прямоугольник");
			objectTypeNames[1] = QObject::tr("Окружность");
			objectTypeNames[2] = QObject::tr("Плоскость");
			objectTypeNames[3] = QObject::tr("Брусок");
			objectTypeNames[4] = QObject::tr("Цилиндр");
			objectTypeNames[5] = QObject::tr("Конус");
			objectTypeNames[6] = QObject::tr("Сфера");
			objectTypeNames[7] = QObject::tr("Еллипс");
			objectTypeNames[8] = QObject::tr("Тор");
		}

		return objectTypeNames[type];
	}

}

