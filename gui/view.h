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

#include <messagereceiver.h>

class QFocusEvent;
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

		enum CurrentAction
		{
			caNone,
			caMove,
			caRotate,
			caRectSelect
		};

		 enum ModifierKey
		 {
			 mkNone,
			 mkSpace
		 };

	public:
		View(QWidget* parent = 0);
		~View();

		void setModel(Model* model);
		Model* getModel() const;

		void setInteractiveView(InteractiveView* view);

		virtual void receive(Common::Message* msg);

		const ViewerShape* getSelectedShape() const;

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
		void shapeAdded(const Shape& shape);
		void shapeRemoved(const Shape& shape);
		void pathSelected(SoPath* path);
		void pathDeselected(SoPath* path);

	private:
		void createUI();
		void init();

		void onLButtonDown(const int flags, const QPoint point);
		void onRButtonDown(const int flags, const QPoint point);
		void onMButtonDown(const int flags, const QPoint point);

		Model* model;

		QPoint pressedPoint;
		QPoint panPoint;

		CurrentAction curAction;
		ModifierKey modKey;

		std::auto_ptr<InventorViewer> inventorViewer;
		std::auto_ptr<InteractiveView> interactiveView;

		std::auto_ptr<ViewProvider> viewProvider;

		const ViewerShape* selectedShape;
	};

}

#endif // VIEW_HEADER

