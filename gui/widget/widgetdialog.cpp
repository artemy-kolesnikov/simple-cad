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

#include "widgetdialog.h"

#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>

namespace Gui
{

	WidgetDialog::WidgetDialog(QWidget* parent) : QDialog(parent),
		layout(0), centralWidget(0)
	{
		createUI();
	}

	void WidgetDialog::createUI()
	{
		layout = new QVBoxLayout(this);

		QHBoxLayout* buttonLayout = new QHBoxLayout();
		layout->addLayout(buttonLayout);

		QPushButton* btOk = new QPushButton(tr("OK"), this);
		connect(btOk, SIGNAL(clicked()), this, SLOT(accept()));

		QPushButton* btCancel = new QPushButton(tr("Cancel"), this);
		connect(btCancel, SIGNAL(clicked()), this, SLOT(reject()));

		buttonLayout->addStretch();
		buttonLayout->addWidget(btOk);
		buttonLayout->addWidget(btCancel);
	}

	void WidgetDialog::setCentralWidget(QWidget* widget)
	{
		if (centralWidget)
			layout->removeWidget(widget);

		centralWidget = widget;

		layout->insertWidget(0, centralWidget);
	}

	QWidget* WidgetDialog::getCentralWidget() const
	{
		return centralWidget;
	}

}

