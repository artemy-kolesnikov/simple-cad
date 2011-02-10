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

#ifndef POINT_SETTINGS_WIDGET_HEADER
#define POINT_SETTINGS_WIDGET_HEADER

#include <QWidget>

#include <gp_Pnt.hxx>

class QDoubleSpinBox;

class PointSettingsWidget : public QWidget
{
	Q_OBJECT
public:
	PointSettingsWidget(QWidget* parent = 0);

	gp_Pnt getPoint() const;
	void setPoint(const gp_Pnt& pt);

private:
	void createUI();

	QDoubleSpinBox* sbXPos;
	QDoubleSpinBox* sbYPos;
	QDoubleSpinBox* sbZPos;
};

#endif // POINT_SETTINGS_WIDGET_HEADER

