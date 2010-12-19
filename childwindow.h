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

#ifndef CHILD_WINDOW_HEADER
#define CHILD_WINDOW_HEADER

#include <QMdiSubWindow>
#include <QString>

class Model;
class View;
class Controller;
class QShapeModel;

/**
 * Child MDI window.
 * Keeps pointers to model, set of views and controller
 */
class ChildWindow : public QMdiSubWindow
{
	Q_OBJECT

public:
	ChildWindow(Model* model, QWidget* parent = 0);

	Controller* getController() const;
	View* getView() const;
	QShapeModel* getShapeModel() const;

Q_SIGNALS:
	void selectionChanged();

protected:
	void keyPressEvent(QKeyEvent*);
	void keyReleaseEvent(QKeyEvent*);

private Q_SLOTS:
	void modelChanged();
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

#endif // CHILD_WINDOW_HEADER
