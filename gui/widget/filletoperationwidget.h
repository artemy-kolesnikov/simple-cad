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

#ifndef FILLET_OPERATION_WIDGET
#define FILLET_OPERATION_WIDGET

#include <QWidget>
#include <QList>
#include <memory>

#include <shape.h>
#include <combineaction.h>

class QDoubleSpinBox;

namespace Gui
{

	class ShapeListWidget;

	class FilletOperationWidget : public QWidget
	{
		Q_OBJECT
	public:
		FilletOperationWidget(const QList<Shape>& shapeList, QWidget* parent = 0);

		void getAction(std::auto_ptr<Action::CombineAction>& action) const;

	private:
		void createUI();

	private:
		ShapeListWidget* shapeListWidget;
		const QList<Shape>& shapeList;

		QDoubleSpinBox* sbRadius;
	};

}

#endif // FILLET_OPERATION_WIDGET

