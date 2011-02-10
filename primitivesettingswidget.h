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

#ifndef PRIMITIVE_SETTINGS_WIDGET_HEADER
#define PRIMITIVE_SETTINGS_WIDGET_HEADER

#include <QWidget>
#include <QStringList>

#include <gp_Pnt.hxx>
#include <gp_Dir.hxx>

class PositionSettingsWidget;
class QComboBox;
class QDoubleSpinBox;
class QLabel;

class PrimitiveSettingsWidget : public QWidget
{
	Q_OBJECT
public:

	enum PrimitiveType
	{
		ptPlane,
		ptBox,
		ptCylinder,
		ptCone,
		ptSphere,
		ptEllipsoid,
		ptTorus
	};

	PrimitiveSettingsWidget(QWidget* parent = 0);

	void setType(PrimitiveType type);
	PrimitiveType getType() const;

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

private Q_SLOTS:
	void primitiveSelectionChanged(int index);

private:
	void createUI();

	PositionSettingsWidget* positionSettings;
	QComboBox* cbPrimitive;
	QStringList primitiveNames;

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
};

#endif // PRIMITIVE_SETTINGS_WIDGET_HEADER

