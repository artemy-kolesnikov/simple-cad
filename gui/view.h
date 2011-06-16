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

#ifndef VIEW_HEADER
#define VIEW_HEADER

#include <QWidget>
#include <QPaintEvent>
#include <QPoint>
#include <memory>
#include <TopoDS_Shape.hxx>

#include <messagereceiver.h>

class SoPath;

namespace Gui
{
	class InventorViewer;
	class InteractiveView;
	class Model;
	class ViewProvider;
	class Shape;
	class ViewerShape;

	/**
	 * Display loaded model
	 */
	class View : public QWidget, public Common::MessageReceiver
	{
		Q_OBJECT
	public:
		View(QWidget* parent = 0);
		~View();

		void setModel(Model* model);
		Model* getModel() const;

		void setInteractiveView(InteractiveView* view);

		virtual void receive(Common::Message* msg);

		const ViewerShape* getSelectedShape() const;

		//TODO: Возможно выбранный элемент нужно хранить в классе Shape или ViewerShape
		TopoDS_Shape getSelectedTopoElement() const;

	Q_SIGNALS:
		void selectionChanged();

	public Q_SLOTS:
		void viewFront();
		void viewBack();
		void viewTop();
		void viewBottom();
		void viewLeft();
		void viewRight();
		void viewAll();
		void viewAxometric();
		void viewDatumPlane();
		void manipulateShape(bool manipulate);

	private Q_SLOTS:
		void shapeAdded(const Shape& shape);
		void shapeRemoved(const Shape& shape);
		void pathSelected(SoPath* path);
		void pathDeselected(SoPath* path);

	private:
		void createUI();

		Model* model;

		std::auto_ptr<InventorViewer> inventorViewer;
		std::auto_ptr<InteractiveView> interactiveView;

		std::auto_ptr<ViewProvider> viewProvider;

		const ViewerShape* selectedShape;
		TopoDS_Shape selectedTopoElement;

		bool isShapeManip;
		bool manipSeted;
	};

}

#endif // VIEW_HEADER

