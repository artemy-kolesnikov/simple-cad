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

#include "shapelistwidget.h"

#include <QComboBox>
#include <QHBoxLayout>

namespace Gui
{

	ShapeListWidget::ShapeListWidget(const QList<Shape>& shapeList,
		QWidget* parent) : QWidget(parent), shapeList(shapeList)
	{
		createUI();
	}

	void ShapeListWidget::createUI()
	{
		QHBoxLayout* layout = new QHBoxLayout(this);

		cbShape = new QComboBox(this);
		layout->addWidget(cbShape);

		QList<Shape>::const_iterator it = shapeList.begin();
		for(;it < shapeList.end(); ++it)
		{
			cbShape->addItem(it->getName());
		}
	}

	const Shape& ShapeListWidget::getShape() const
	{
		return shapeList.at(cbShape->currentIndex());
	}

	void ShapeListWidget::setSelectedShape(const Shape& shape)
	{
		int index = shapeList.indexOf(shape);
		if (index != -1)
			cbShape->setCurrentIndex(index);
	}
}

