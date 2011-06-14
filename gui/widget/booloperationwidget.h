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

#ifndef BOOL_OPERATION_WIDGET_HEADER
#define BOOL_OPERATION_WIDGET_HEADER

#include <QWidget>
#include <QList>
#include <memory>

#include <shape.h>
#include <boolean.h>
#include <boolcommand.h>

class QComboBox;
class QRadioButton;

namespace Gui
{

	class ShapeListWidget;

	class BoolOperationWidget : public QWidget
	{
		Q_OBJECT
	public:
		BoolOperationWidget(const QList<Shape>& shapeList, BooleanCommand::OperationType type,
			QWidget* parent = 0);

		void getAction(std::auto_ptr<Action::Boolean>& action) const;

	private:
		void createUI();

	private:
		BooleanCommand::OperationType type;

		ShapeListWidget* shapeList1;
		ShapeListWidget* shapeList2;

		QRadioButton* rbFuse;
		QRadioButton* rbCommon;
		QRadioButton* rbCut;

		const QList<Shape>& shapeList;
	};

}

#endif // BOOL_OPERATION_WIDGET_HEADER

