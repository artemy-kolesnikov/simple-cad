/*
 * Simple CAD System
 * Based on opencascade (www.opencascade.org)
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

#include "positionsettingswidget.h"

#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QComboBox>

#include "pointsettingswidget.h"

namespace Gui
{

	PositionSettingsWidget::PositionSettingsWidget(QWidget* parent) : QWidget(parent)
	{
		createUI();
	}

	void PositionSettingsWidget::createUI()
	{
		QVBoxLayout* layout = new QVBoxLayout(this);

		pointWidget = new PointSettingsWidget(this);
		layout->addWidget(pointWidget);

		QHBoxLayout* dirLayout = new QHBoxLayout();
		layout->addLayout(dirLayout);

		QLabel* lbDirection = new QLabel(tr("Direction:"), this);
		dirLayout->addWidget(lbDirection);

		cbDirection = new QComboBox(this);
		cbDirection->addItem("X");
		cbDirection->addItem("Y");
		cbDirection->addItem("Z");
		//cbDirection->addItem(tr("Пользовательская"));
		dirLayout->addWidget(cbDirection);
	}

	gp_Pnt PositionSettingsWidget::getPoint() const
	{
		return pointWidget->getPoint();
	}

	void PositionSettingsWidget::setPoint(const gp_Pnt& pt)
	{
		pointWidget->setPoint(pt);
	}

	gp_Dir PositionSettingsWidget::getDirection() const
	{
		if (cbDirection->currentIndex() == 0)
			return gp::DX();
		if (cbDirection->currentIndex() == 1)
			return gp::DY();
		if (cbDirection->currentIndex() == 2)
			return gp::DZ();
	}

	void PositionSettingsWidget::setDirection(const gp_Dir& dir)
	{
		if (dir.IsParallel(gp::DX(), 0))
			cbDirection->setCurrentIndex(0);
		else if (dir.IsParallel(gp::DY(), 0))
			cbDirection->setCurrentIndex(1);
		else if (dir.IsParallel(gp::DZ(), 0))
			cbDirection->setCurrentIndex(2);
	}

}

