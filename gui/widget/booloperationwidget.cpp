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

#include "booloperationwidget.h"

#include <QVBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QRadioButton>

#include <TopoDS_Shape.hxx>

#include <fuse.h>
#include <common.h>
#include <cut.h>

#include "shapelistwidget.h"

namespace Gui
{

	BoolOperationWidget::BoolOperationWidget(const QList<Shape>& shapeList, BooleanCommand::OperationType type,
		QWidget* parent) : QWidget(parent), shapeList(shapeList), type(type)
	{
		createUI();
	}

	void BoolOperationWidget::getAction(std::auto_ptr<Action::Boolean>& action) const
	{
		TopoDS_Shape shape1 = shapeList1->getShape().getShape();
		TopoDS_Shape shape2 = shapeList2->getShape().getShape();

		if (rbFuse->isChecked())
			action.reset(new Action::Fuse(shape1, shape2));
		else if (rbCommon->isChecked())
			action.reset(new Action::Common(shape1, shape2));
		else if (rbCut->isChecked())
			action.reset(new Action::Cut(shape1, shape2));
	}

	void BoolOperationWidget::createUI()
	{
		QVBoxLayout* layout = new QVBoxLayout(this);

		QLabel* lbOpType = new QLabel(tr("Операция"), this);
		layout->addWidget(lbOpType);

		QVBoxLayout* opLayout = new QVBoxLayout();
		layout->addLayout(opLayout);

		rbFuse = new QRadioButton(BooleanCommand::getOperationNameByType(BooleanCommand::Fuse), this);
		rbFuse->setChecked(true);
		opLayout->addWidget(rbFuse);

		rbCommon = new QRadioButton(BooleanCommand::getOperationNameByType(BooleanCommand::Common), this);
		opLayout->addWidget(rbCommon);

		rbCut = new QRadioButton(BooleanCommand::getOperationNameByType(BooleanCommand::Cut), this);
		opLayout->addWidget(rbCut);

		QLabel* lbShape = new QLabel(tr("Фигура"), this);
		layout->addWidget(lbShape);

		QVBoxLayout* shapeLayout = new QVBoxLayout();
		layout->addLayout(shapeLayout);

		shapeList1 = new ShapeListWidget(shapeList, this);
		shapeLayout->addWidget(shapeList1);

		shapeList2 = new ShapeListWidget(shapeList, this);
		shapeLayout->addWidget(shapeList2);
	}

}

