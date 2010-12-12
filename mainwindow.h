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

#ifndef MAIN_WINDOW_HEADER
#define MAIN_WINDOW_HEADER

#include <QMainWindow>
#include <QList>
#include <QMap>

class QMdiArea;
class QDockWidget;
class QMenuBar;
class QAction;
class QTreeView;
class Model;
class QToolBar;
class QMdiSubWindow;

class ChildWindow;

class MainWindow : public QMainWindow
{
	Q_OBJECT
public:
	MainWindow(QWidget* parent = 0);

private Q_SLOTS:
	void shapesVisChanged(bool);
	void about();
	void openModel();
	ChildWindow* newChildWindow();
	void childWindowActivated(QMdiSubWindow* window);
	void modelChanged();

private:
	void createUI();
	void createDockWidget();
	void createMenuAndActions();

	QMdiArea* mdiArea;

	// Model shapes list
	QDockWidget* shapesDock;
	QMenuBar* mainMenuBar;
	QToolBar* mainToolBar;
	QTreeView* shapesTreeView;

	QAction* acExit;
	QAction* acAbout;
	QAction* acOpenModel;
	QAction* acNewModel;

	QAction* acLine;
	QAction* acCurve;
	QAction* acEllipse;
	QAction* acRectangle;
	QAction* acFunction;
	QAction* acStamp;
	QAction* acRotation;
	QAction* acAddition;
	QAction* acSubstract;

	QMap<QWidget*, Model*> windowModelMap;

	ChildWindow* currentChild;
};

#endif // MAIN_WINDOW_HEADER
