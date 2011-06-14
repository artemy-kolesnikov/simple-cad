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

#include "filletoperationwidget.h"

#include <QDoubleSpinBox>
#include <QGridLayout>
#include <QLabel>

#include <TopoDS_Shape.hxx>

#include <fillet.h>

#include "shapelistwidget.h"

namespace Gui
{

	FilletOperationWidget::FilletOperationWidget(const QList<Shape>& shapeList, QWidget* parent) :
		QWidget(parent), shapeList(shapeList)
	{
		createUI();
	}

	void FilletOperationWidget::createUI()
	{
		QGridLayout* layout = new QGridLayout(this);

		QLabel* lbRadius = new QLabel(tr("Радиус"), this);
		layout->addWidget(lbRadius, 0, 0);

		sbRadius = new QDoubleSpinBox(this);
		sbRadius->setMinimum(0.01);
		layout->addWidget(sbRadius, 0, 1);

		QLabel* lbShape = new QLabel(tr("Фигура"), this);
		layout->addWidget(lbShape, 1, 0);

		shapeListWidget = new ShapeListWidget(shapeList, this);
		layout->addWidget(shapeListWidget, 1, 1);
	}

	void FilletOperationWidget::getAction(std::auto_ptr<Action::CombineAction>& action) const
	{
		TopoDS_Shape shape = shapeListWidget->getShape().getShape();
		float radius = sbRadius->value();

		action.reset(new Action::Fillet(shape, radius));
	}

}

