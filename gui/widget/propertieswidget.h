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

#ifndef PROPERTIES_WIDGET_HEADER
#define PROPERTIES_WIDGET_HEADER

#include <QWidget>

#include <Graphic3d_NameOfMaterial.hxx>
#include <AIS_Shape.hxx>

class QComboBox;
class QCheckBox;

namespace Gui
{

	class Model;

	/**
	 * Model properties widget
	 */
	class PropertiesWidget : public QWidget
	{
		Q_OBJECT
	public:
		PropertiesWidget(QWidget* parent = 0);

		void setModel(Model* model);
		void setShape(Handle(AIS_Shape)& shape);

	Q_SIGNALS:
		void materialChanged(Graphic3d_NameOfMaterial material);
		void shadedChanged(bool shaded);

	private Q_SLOTS:
		void materialChanged(int);
		void shaddedChanged(int);

	private:
		void createUI();
		void updateView();

	private:
		Model* model;
		Handle(AIS_Shape) shape;

		QComboBox* cbMaterial;
		QCheckBox* cbShaded;
	};

}

#endif // PROPERTIES_WIDGET_HEADER
