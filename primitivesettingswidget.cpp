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

#include "primitivesettingswidget.h"
#include "positionsettingswidget.h"

#include <QComboBox>
#include <QGridLayout>
#include <QDoubleSpinBox>
#include <QLabel>

PrimitiveSettingsWidget::PrimitiveSettingsWidget(QWidget* parent) : QWidget(parent)
{
	primitiveNames << tr("Плоскость")
		<< tr("Брусок")
		<< tr("Цилиндр")
		<< tr("Конус")
		<< tr("Сфера")
		<< tr("Еллипс")
		<< tr("Тор");

	createUI();
}

void PrimitiveSettingsWidget::createUI()
{
	QGridLayout* layout = new QGridLayout(this);

	cbPrimitive = new QComboBox(this);
	cbPrimitive->setCurrentIndex(-1);
	connect(cbPrimitive, SIGNAL(activated(int)), this,
		SLOT(primitiveSelectionChanged(int)));
	cbPrimitive->addItems(primitiveNames);
	layout->addWidget(cbPrimitive, 0, 0);

	positionSettings = new PositionSettingsWidget(this);
	layout->addWidget(positionSettings, 1, 0);

	QWidget* paramsWidget = new QWidget(this);
	layout->addWidget(paramsWidget, 1, 1);

	QGridLayout* paramsLayout = new QGridLayout(paramsWidget);

	lbWidth = new QLabel(tr("Ширина:"), this);
	paramsLayout->addWidget(lbWidth, 0, 0);

	sbWidth = new QDoubleSpinBox(this);
	sbWidth->setMaximum(9999);
	paramsLayout->addWidget(sbWidth, 0, 1);

	lbHeight = new QLabel(tr("Высота:"), this);
	paramsLayout->addWidget(lbHeight, 1, 0);

	sbHeight = new QDoubleSpinBox(this);
	sbHeight->setMaximum(9999);
	paramsLayout->addWidget(sbHeight, 1, 1);

	lbLength = new QLabel(tr("Длина:"), this);
	paramsLayout->addWidget(lbLength, 2, 0);

	sbLength = new QDoubleSpinBox(this);
	sbLength->setMaximum(9999);
	paramsLayout->addWidget(sbLength, 2, 1);

	lbRadius1 = new QLabel(tr("Радиус 1:"), this);
	paramsLayout->addWidget(lbRadius1, 3, 0);

	sbRadius1 = new QDoubleSpinBox(this);
	sbRadius1->setMaximum(9999);
	paramsLayout->addWidget(sbRadius1, 3, 1);

	lbRadius2 = new QLabel(tr("Радиус 2:"), this);
	paramsLayout->addWidget(lbRadius2, 4, 0);

	sbRadius2 = new QDoubleSpinBox(this);
	sbRadius2->setMaximum(9999);
	paramsLayout->addWidget(sbRadius2, 4, 1);

	lbAngle = new QLabel(tr("Угол:"), this);
	paramsLayout->addWidget(lbAngle, 5, 0);

	sbAngle = new QDoubleSpinBox(this);
	sbAngle->setMaximum(9999);
	paramsLayout->addWidget(sbAngle, 5, 1);
}

void PrimitiveSettingsWidget::setType(PrimitiveType type)
{
	cbPrimitive->setCurrentIndex(type);
	primitiveSelectionChanged(type);
}

PrimitiveSettingsWidget::PrimitiveType PrimitiveSettingsWidget::getType() const
{
	return static_cast<PrimitiveType>(cbPrimitive->currentIndex());
}

gp_Pnt PrimitiveSettingsWidget::getPoint() const
{
	return positionSettings->getPoint();
}

void PrimitiveSettingsWidget::setPoint(const gp_Pnt& pt)
{
	positionSettings->setPoint(pt);
}

gp_Dir PrimitiveSettingsWidget::getDir() const
{
	return positionSettings->getDirection();
}

void PrimitiveSettingsWidget::setDir(const gp_Dir& dir)
{
	positionSettings->setDirection(dir);
}

float PrimitiveSettingsWidget::getWidth() const
{
	return sbWidth->value();
}

void PrimitiveSettingsWidget::setWidth(float value)
{
	sbWidth->setValue(value);
}

float PrimitiveSettingsWidget::getHeight() const
{
	return sbHeight->value();
}

void PrimitiveSettingsWidget::setHeight(float value)
{
	sbHeight->setValue(value);
}

float PrimitiveSettingsWidget::getLength() const
{
	return sbLength->value();
}

void PrimitiveSettingsWidget::setLength(float value)
{
	sbLength->setValue(value);
}

float PrimitiveSettingsWidget::getRadius1() const
{
	return sbRadius1->value();
}

void PrimitiveSettingsWidget::setRadius1(float value)
{
	sbRadius1->setValue(value);
}

float PrimitiveSettingsWidget::getRadius2() const
{
	return sbRadius2->value();
}

void PrimitiveSettingsWidget::setRadius2(float value)
{
	sbRadius2->setValue(value);
}

float PrimitiveSettingsWidget::getAngle() const
{
	return sbAngle->value();
}

void PrimitiveSettingsWidget::setAngle(float value)
{
	sbAngle->setValue(value);
}

void PrimitiveSettingsWidget::primitiveSelectionChanged(int index)
{
	//TODO: Переделать принцип отображения параметров

	sbWidth->setVisible(true);
	sbHeight->setVisible(true);
	sbLength->setVisible(true);
	sbRadius1->setVisible(true);
	sbRadius2->setVisible(true);
	sbAngle->setVisible(true);

	lbWidth->setVisible(true);
	lbHeight->setVisible(true);
	lbLength->setVisible(true);
	lbRadius1->setVisible(true);
	lbRadius2->setVisible(true);
	lbAngle->setVisible(true);

	switch(index)
	{
		case ptPlane:
			sbLength->setVisible(false);
			sbRadius1->setVisible(false);
			sbRadius2->setVisible(false);
			sbAngle->setVisible(false);

			lbLength->setVisible(false);
			lbRadius1->setVisible(false);
			lbRadius2->setVisible(false);
			lbAngle->setVisible(false);
			break;
		case ptBox:
			sbRadius1->setVisible(false);
			sbRadius2->setVisible(false);
			sbAngle->setVisible(false);

			lbRadius1->setVisible(false);
			lbRadius2->setVisible(false);
			lbAngle->setVisible(false);
			break;
		case ptCylinder:
			sbWidth->setVisible(false);
			sbLength->setVisible(false);
			sbRadius2->setVisible(false);

			lbWidth->setVisible(false);
			lbLength->setVisible(false);
			lbRadius2->setVisible(false);
			break;
		case ptCone:
			sbWidth->setVisible(false);
			sbLength->setVisible(false);

			lbWidth->setVisible(false);
			lbLength->setVisible(false);
			break;
		case ptSphere:
			sbWidth->setVisible(false);
			sbHeight->setVisible(false);
			sbLength->setVisible(false);
			sbRadius2->setVisible(false);

			lbWidth->setVisible(false);
			lbHeight->setVisible(false);
			lbLength->setVisible(false);
			lbRadius2->setVisible(false);
			break;
		case ptEllipsoid:
			sbWidth->setVisible(false);
			sbHeight->setVisible(false);
			sbLength->setVisible(false);

			lbWidth->setVisible(false);
			lbHeight->setVisible(false);
			lbLength->setVisible(false);
			break;
		case ptTorus:
			sbWidth->setVisible(false);
			sbHeight->setVisible(false);
			sbLength->setVisible(false);

			lbWidth->setVisible(false);
			lbHeight->setVisible(false);
			lbLength->setVisible(false);
			break;
	}
}

