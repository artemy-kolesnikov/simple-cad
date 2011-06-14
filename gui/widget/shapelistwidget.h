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

#ifndef SHAPE_LIST_WIDGET_HEADER
#define SHAPE_LIST_WIDGET_HEADER

#include <QWidget>
#include <QList>

#include <shape.h>

class QComboBox;

namespace Gui
{

	class ShapeListWidget : public QWidget
	{
		Q_OBJECT
	public:
		ShapeListWidget(const QList<Shape>& shapeList, QWidget* parent = 0);

		const Shape& getShape() const;

	private:
		void createUI();

	private:
		const QList<Shape>& shapeList;
		QComboBox* cbShape;
	};

}

#endif // SHAPE_LIST_WIDGET_HEADER

