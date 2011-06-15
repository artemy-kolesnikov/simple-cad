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

#ifndef CHILD_WINDOW_HEADER
#define CHILD_WINDOW_HEADER

#include <QMdiSubWindow>
#include <QString>

class QShapeModel;
class QCloseEvent;

namespace Gui
{

	class View;
	class Controller;
	class Model;

	/**
	 * Child MDI window.
	 * Keeps pointers to view and controller
	 */
	class ChildWindow : public QMdiSubWindow
	{
		Q_OBJECT

	public:
		ChildWindow(Model* model, QWidget* parent = 0);

		Controller* getController() const;
		View& getView() const;
		QShapeModel* getShapeModel() const;

	Q_SIGNALS:
		void selectionChanged();
		void closed();

	protected:
		virtual void closeEvent(QCloseEvent* event);

	private Q_SLOTS:
		void modelFileNameChanged(QString& fileName);

	private:
		void createUI();

		// In usual model is not need
		//Model* getModel() const;

		Model* model;
		View* view;
		Controller* controller;
		QShapeModel* shapeModel;
	};

}

#endif // CHILD_WINDOW_HEADER
