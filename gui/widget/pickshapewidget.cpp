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

#include "pickshapewidget.h"

#include <QCoreApplication>
#include <QDialog>
#include <QHBoxLayout>
#include <QPushButton>
#include <QVBoxLayout>

#include <cassert>

#include <view.h>
#include <viewershape.h>

namespace Gui
{
	PickShapeWidget::PickShapeWidget(const View& view, QWidget* parent) :
		QWidget(parent), centralWidget(0)
	{
		createUI();

		connect(&view, SIGNAL(selectionChanged()),
			this, SLOT(shapeSelectionChanged()));
	}

	void PickShapeWidget::createUI()
	{
		layout = new QVBoxLayout(this);

		QHBoxLayout* buttonLayout = new QHBoxLayout();
		layout->addLayout(buttonLayout);

		QPushButton* btOk = new QPushButton(tr("OK"), this);
		connect(btOk, SIGNAL(clicked()), this, SIGNAL(confirmed()));

		QPushButton* btCancel = new QPushButton(tr("Cancel"), this);
		connect(btCancel, SIGNAL(clicked()), this, SIGNAL(canceled()));

		buttonLayout->addStretch();
		buttonLayout->addWidget(btOk);
		buttonLayout->addWidget(btCancel);

		layout->addStretch();
	}

	void PickShapeWidget::shapeSelectionChanged()
	{
		const View* view = static_cast<View*>(sender());
		assert(view);

		const ViewerShape* shape = view->getSelectedShape();
		// selection cleared
		if (!shape)
			return;

		TopoDS_Shape topoElement = view->getSelectedTopoElement();
		assert(!topoElement.IsNull());

		Q_EMIT shapeSelected(*shape, topoElement);
	}

	void PickShapeWidget::setCentralWidget(QWidget* widget)
	{
		if (centralWidget)
			layout->removeWidget(widget);

		centralWidget = widget;

		layout->insertWidget(1, centralWidget);
	}

	QWidget* PickShapeWidget::getCentralWidget() const
	{
		return centralWidget;
	}

}

