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

#include "createprimitivewidget.h"
#include "positionsettingswidget.h"

#include <createbox.h>
#include <createcone.h>
#include <createcylinder.h>
#include <createellipsoid.h>
#include <createplane.h>
#include <createrectangle.h>
#include <createrectangle.h>
#include <createsphere.h>
#include <createtorus.h>

#include <QComboBox>
#include <QGridLayout>
#include <QDoubleSpinBox>
#include <QLabel>

namespace Gui
{

	CreatePrimitiveWidget::CreatePrimitiveWidget(QWidget* parent) : QWidget(parent)
	{
		createUI();
	}

	void CreatePrimitiveWidget::createUI()
	{
		QGridLayout* layout = new QGridLayout(this);

		cbPrimitive = new QComboBox(this);
		cbPrimitive->setCurrentIndex(-1);
		connect(cbPrimitive, SIGNAL(activated(int)), this,
			SLOT(primitiveSelectionChanged(int)));
		cbPrimitive->addItems(CreatePrimitiveCommand::getObjectNames());
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
		sbWidth->setMinimum(1);
		paramsLayout->addWidget(sbWidth, 0, 1);

		lbHeight = new QLabel(tr("Высота:"), this);
		paramsLayout->addWidget(lbHeight, 1, 0);

		sbHeight = new QDoubleSpinBox(this);
		sbHeight->setMaximum(9999);
		sbHeight->setMinimum(1);
		paramsLayout->addWidget(sbHeight, 1, 1);

		lbLength = new QLabel(tr("Длина:"), this);
		paramsLayout->addWidget(lbLength, 2, 0);

		sbLength = new QDoubleSpinBox(this);
		sbLength->setMaximum(9999);
		sbLength->setMinimum(1);
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

	void CreatePrimitiveWidget::setType(CreatePrimitiveCommand::ObjectType type)
	{
		cbPrimitive->setCurrentIndex(type);
		primitiveSelectionChanged(type);

		this->type = type;
	}
 
	Action::Create3dPrimitive* CreatePrimitiveWidget::createRectangleAction() const
	{
		return 0;
	}

	Action::Create3dPrimitive* CreatePrimitiveWidget::createCircleAction() const
	{
		return 0;
	}

	Action::Create3dPrimitive* CreatePrimitiveWidget::createPlaneAction() const
	{
		gp_Ax3 axis(getPoint(), getDir());
		float height = getHeight();
		float width = getWidth();
		return new Action::CreatePlane(axis, height, width);
	}

	Action::Create3dPrimitive* CreatePrimitiveWidget::createBoxAction() const
	{
		gp_Ax3 axis(getPoint(), getDir());
		float height = getHeight();
		float width = getWidth();
		float lenght = getLength();

		return new Action::CreateBox(axis, height, width, lenght);
	}

	Action::Create3dPrimitive* CreatePrimitiveWidget::createCylinderAction() const
	{
		gp_Ax3 axis(getPoint(), getDir());
		float radius = getRadius1();
		float height = getHeight();
		float angle = getAngle();

		return new Action::CreateCylinder(axis, radius, height, angle);
	}

	Action::Create3dPrimitive* CreatePrimitiveWidget::createConeAction() const
	{
		gp_Ax3 axis(getPoint(), getDir());
		float radius1 = getRadius1();
		float radius2 = getRadius2();
		float height = getHeight();
		float angle = getAngle();
		return new Action::CreateCone(axis, radius1, radius2, height, angle);
	}

	Action::Create3dPrimitive* CreatePrimitiveWidget::createSphereAction() const
	{
		gp_Ax3 axis(getPoint(), getDir());
		float radius = getRadius1();
		float angle = getAngle();
		return new Action::CreateSphere(axis, radius, angle);
	}

	Action::Create3dPrimitive* CreatePrimitiveWidget::createEllipsoidAction() const
	{
		gp_Ax3 axis(getPoint(), getDir());
		float height = getHeight();
		float width = getWidth();
		return new Action::CreatePlane(axis, height, width);
	}

	Action::Create3dPrimitive* CreatePrimitiveWidget::createTorusAction() const
	{
		gp_Ax3 axis(getPoint(), getDir());
		float radius1 = getRadius1();
		float radius2 = getRadius2();
		float angle = getAngle();
		return new Action::CreateTorus(axis, radius1, radius2, angle, 0, 0);
	}

	void CreatePrimitiveWidget::getAction(std::auto_ptr<Action::Create3dPrimitive>& action) const
	{
		switch (type)
		{
			case CreatePrimitiveCommand::Rectangle:
				action.reset(createRectangleAction());
				break;
			case CreatePrimitiveCommand::Circle:
				action.reset(createCircleAction());
				break;
			case CreatePrimitiveCommand::Plane:
				action.reset(createPlaneAction());
				break;
			case CreatePrimitiveCommand::Box:
				action.reset(createBoxAction());
				break;
			case CreatePrimitiveCommand::Cylinder:
				action.reset(createCylinderAction());
				break;
			case CreatePrimitiveCommand::Cone:
				action.reset(createConeAction());
				break;
			case CreatePrimitiveCommand::Sphere:
				action.reset(createSphereAction());
				break;
			case CreatePrimitiveCommand::Ellipsoid:
				action.reset(createEllipsoidAction());
				break;
			case CreatePrimitiveCommand::Torus:
				action.reset(createTorusAction());
				break;
		}
	}

	CreatePrimitiveCommand::ObjectType CreatePrimitiveWidget::getType() const
	{
		return static_cast<CreatePrimitiveCommand::ObjectType>(cbPrimitive->currentIndex());
	}

	gp_Pnt CreatePrimitiveWidget::getPoint() const
	{
		return positionSettings->getPoint();
	}

	void CreatePrimitiveWidget::setPoint(const gp_Pnt& pt)
	{
		positionSettings->setPoint(pt);
	}

	gp_Dir CreatePrimitiveWidget::getDir() const
	{
		return positionSettings->getDirection();
	}

	void CreatePrimitiveWidget::setDir(const gp_Dir& dir)
	{
		positionSettings->setDirection(dir);
	}

	float CreatePrimitiveWidget::getWidth() const
	{
		return sbWidth->value();
	}

	void CreatePrimitiveWidget::setWidth(float value)
	{
		sbWidth->setValue(value);
	}

	float CreatePrimitiveWidget::getHeight() const
	{
		return sbHeight->value();
	}

	void CreatePrimitiveWidget::setHeight(float value)
	{
		sbHeight->setValue(value);
	}

	float CreatePrimitiveWidget::getLength() const
	{
		return sbLength->value();
	}

	void CreatePrimitiveWidget::setLength(float value)
	{
		sbLength->setValue(value);
	}

	float CreatePrimitiveWidget::getRadius1() const
	{
		return sbRadius1->value();
	}

	void CreatePrimitiveWidget::setRadius1(float value)
	{
		sbRadius1->setValue(value);
	}

	float CreatePrimitiveWidget::getRadius2() const
	{
		return sbRadius2->value();
	}

	void CreatePrimitiveWidget::setRadius2(float value)
	{
		sbRadius2->setValue(value);
	}

	float CreatePrimitiveWidget::getAngle() const
	{
		return sbAngle->value();
	}

	void CreatePrimitiveWidget::setAngle(float value)
	{
		sbAngle->setValue(value);
	}

	void CreatePrimitiveWidget::primitiveSelectionChanged(int index)
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
			case CreatePrimitiveCommand::Plane:
				sbLength->setVisible(false);
				sbRadius1->setVisible(false);
				sbRadius2->setVisible(false);
				sbAngle->setVisible(false);

				lbLength->setVisible(false);
				lbRadius1->setVisible(false);
				lbRadius2->setVisible(false);
				lbAngle->setVisible(false);
				break;
			case CreatePrimitiveCommand::Box:
				sbRadius1->setVisible(false);
				sbRadius2->setVisible(false);
				sbAngle->setVisible(false);

				lbRadius1->setVisible(false);
				lbRadius2->setVisible(false);
				lbAngle->setVisible(false);
				break;
			case CreatePrimitiveCommand::Cylinder:
				sbWidth->setVisible(false);
				sbLength->setVisible(false);
				sbRadius2->setVisible(false);

				lbWidth->setVisible(false);
				lbLength->setVisible(false);
				lbRadius2->setVisible(false);
				break;
			case CreatePrimitiveCommand::Cone:
				sbWidth->setVisible(false);
				sbLength->setVisible(false);

				lbWidth->setVisible(false);
				lbLength->setVisible(false);
				break;
			case CreatePrimitiveCommand::Sphere:
				sbWidth->setVisible(false);
				sbHeight->setVisible(false);
				sbLength->setVisible(false);
				sbRadius2->setVisible(false);

				lbWidth->setVisible(false);
				lbHeight->setVisible(false);
				lbLength->setVisible(false);
				lbRadius2->setVisible(false);
				break;
			case CreatePrimitiveCommand::Ellipsoid:
				sbWidth->setVisible(false);
				sbHeight->setVisible(false);
				sbLength->setVisible(false);

				lbWidth->setVisible(false);
				lbHeight->setVisible(false);
				lbLength->setVisible(false);
				break;
			case CreatePrimitiveCommand::Torus:
				sbWidth->setVisible(false);
				sbHeight->setVisible(false);
				sbLength->setVisible(false);

				lbWidth->setVisible(false);
				lbHeight->setVisible(false);
				lbLength->setVisible(false);
				break;
		}
	}

}

