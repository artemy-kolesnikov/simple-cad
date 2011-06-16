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

#ifndef PICK_SHAPE_WIDGET
#define PICK_SHAPE_WIDGET

#include <QWidget>
#include <TopoDS_Shape.hxx>

class QVBoxLayout;

namespace Gui
{

	class ViewerShape;
	class View;

	class PickShapeWidget : public QWidget
	{
		Q_OBJECT
	public:
		PickShapeWidget(const View& view, QWidget* parent = 0);
		virtual ~PickShapeWidget() {}

		void setCentralWidget(QWidget* widget);
		QWidget* getCentralWidget() const;

	Q_SIGNALS:
		void shapeSelected(const ViewerShape& shape,
			const TopoDS_Shape& topoElement);
		void confirmed();
		void canceled();

	private:
		void createUI();

	private Q_SLOTS:
		void shapeSelectionChanged();

	private:
		QVBoxLayout* layout;
		QWidget* centralWidget;
	};

}

#endif // PICK_SHAPE_WIDGET

