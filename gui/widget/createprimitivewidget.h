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

#ifndef CREATE_PRIMITIVE_WIDGET_HEADER
#define CREATE_PRIMITIVE_WIDGET_HEADER

#include <QWidget>

#include <gp_Pnt.hxx>
#include <gp_Dir.hxx>

#include <memory>

#include <create3dprimitive.h>
#include <createprimitivecommand.h>

class QComboBox;
class QDoubleSpinBox;
class QLabel;

namespace Gui
{

	class PositionSettingsWidget;

	class CreatePrimitiveWidget : public QWidget
	{
		Q_OBJECT
	public:
		CreatePrimitiveWidget(QWidget* parent = 0);

		void setType(CreatePrimitiveCommand::ObjectType type);
		CreatePrimitiveCommand::ObjectType getType() const;

		void getAction(std::auto_ptr<Action::Create3dPrimitive>& action) const;

	private Q_SLOTS:
		void primitiveSelectionChanged(int index);

	private:
		void createUI();

		gp_Pnt getPoint() const;
		void setPoint(const gp_Pnt& pt);

		gp_Dir getDir() const;
		void setDir(const gp_Dir& dir);

		float getWidth() const;
		void setWidth(float value);

		float getHeight() const;
		void setHeight(float value);

		float getLength() const;
		void setLength(float value);

		float getRadius1() const;
		void setRadius1(float value);

		float getRadius2() const;
		void setRadius2(float value);

		float getAngle() const;
		void setAngle(float value);

		Action::Create3dPrimitive* createRectangleAction() const;
		Action::Create3dPrimitive* createCircleAction() const;
		Action::Create3dPrimitive* createPlaneAction() const;
		Action::Create3dPrimitive* createBoxAction() const;
		Action::Create3dPrimitive* createCylinderAction() const;
		Action::Create3dPrimitive* createConeAction() const;
		Action::Create3dPrimitive* createSphereAction() const;
		Action::Create3dPrimitive* createEllipsoidAction() const;
		Action::Create3dPrimitive* createTorusAction() const;

	private:
		PositionSettingsWidget* positionSettings;
		QComboBox* cbPrimitive;

		QDoubleSpinBox* sbWidth;
		QDoubleSpinBox* sbHeight;
		QDoubleSpinBox* sbLength;
		QDoubleSpinBox* sbRadius1;
		QDoubleSpinBox* sbRadius2;
		QDoubleSpinBox* sbAngle;

		QLabel* lbWidth;
		QLabel* lbHeight;
		QLabel* lbLength;
		QLabel* lbRadius1;
		QLabel* lbRadius2;
		QLabel* lbAngle;

		CreatePrimitiveCommand::ObjectType type;
	};

}

#endif // CREATE_PRIMITIVE_WIDGET_HEADER

