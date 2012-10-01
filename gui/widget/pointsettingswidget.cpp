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

#include "pointsettingswidget.h"

#include <QDoubleSpinBox>
#include <QLabel>
#include <QGridLayout>

namespace Gui
{

	PointSettingsWidget::PointSettingsWidget(QWidget* parent) : QWidget(parent)
	{
		createUI();
	}

	void PointSettingsWidget::createUI()
	{
		QGridLayout* layout = new QGridLayout(this);

		QLabel* lbX = new QLabel(tr("X:"), this);
		layout->addWidget(lbX, 0, 0);

		sbXPos = new QDoubleSpinBox(this);
		sbXPos->setMaximum(99999);
		layout->addWidget(sbXPos, 0, 1);

		QLabel* lbY = new QLabel(tr("Y:"), this);
		layout->addWidget(lbY, 1, 0);

		sbYPos = new QDoubleSpinBox(this);
		sbYPos->setMaximum(99999);
		layout->addWidget(sbYPos, 1, 1);

		QLabel* lbZ = new QLabel(tr("Z:"), this);
		layout->addWidget(lbZ, 2, 0);

		sbZPos = new QDoubleSpinBox(this);
		sbZPos->setMaximum(99999);
		layout->addWidget(sbZPos, 2, 1);
	}

	gp_Pnt PointSettingsWidget::getPoint() const
	{
		return gp_Pnt(sbXPos->value(), sbYPos->value(), sbZPos->value());
	}

	void PointSettingsWidget::setPoint(const gp_Pnt& pt)
	{
		sbXPos->setValue(pt.X());
		sbYPos->setValue(pt.Y());
		sbZPos->setValue(pt.Z());
	}

}

