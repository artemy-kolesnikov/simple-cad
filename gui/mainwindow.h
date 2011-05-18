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

#ifndef MAIN_WINDOW_HEADER
#define MAIN_WINDOW_HEADER

#include <QMainWindow>
#include <QList>
#include <QMap>
#include <QStringList>

class QAction;
class QDockWidget;
class QMdiArea;
class QMdiSubWindow;
class QMenuBar;
class QToolBar;
class QTreeView;

namespace Gui
{

	class ChildWindow;
	class PropertiesWidget;
	class Controller;
	class Model;

	class MainWindow : public QMainWindow
	{
		Q_OBJECT
	public:
		MainWindow(QWidget* parent = 0);

		void openFiles(QStringList& files);

		/*virtual Model* getActiveModel() const;
		virtual View* getActiveView() const;*/

	private Q_SLOTS:
		void shapesVisChanged(bool);
		void about();
		void openModel();
		ChildWindow* newChildWindow();
		void childWindowActivated(QMdiSubWindow* window);
		void modelChanged();
		void setShadded(bool shadded);
		void viewSelectionChanged();
		void makePrism();

		void setDatumPlane();
		void showDatumPlane();
		void hideDatumPlane();

		void viewFront();
		void viewBack();
		void viewTop();
		void viewBottom();
		void viewLeft();
		void viewRight();
		void viewDatumPlane();

		void selectNeutral();
		void selectVertex();
		void selectEdge();
		void selectFace();
		void selectSolid();

		void createBox();
		void createCylinder();
		void createSphere();
		void createCone();
		void createTorus();
		void createPlane();
		void createEllipsoid();

	private:
		void createUI();
		void createDockWidget();
		void createMenu();
		void createCommonActions();
		void createOperationActions();
		void createViewActions();

		ChildWindow* currentChildWindow() const;
		Model* currentModel() const;

		QMdiArea* mdiArea;

		// Model shapes list
		QDockWidget* shapesDock;
		// Model properties
		QDockWidget* propertiesDock;
		QMenuBar* mainMenuBar;
		QToolBar* mainToolBar;
		QTreeView* shapesTreeView;

		QMenu* fileMenu;
		QMenu* actionMenu;
		QMenu* planeMenu;
		QMenu* viewMenu;
		QMenu* helpMenu;
		QMenu* creationMenu;
		QMenu* operationMenu;

		/*QAction* acExit;
		QAction* acAbout;
		QAction* acOpenModel;
		QAction* acNewModel;

		QAction* acViewFront;
		QAction* acViewBack;
		QAction* acViewTop;
		QAction* acViewBottom;
		QAction* acViewLeft;
		QAction* acViewRight;
		QAction* acViewDatumPlane;

		QAction* acSetDatumPlane;
		QAction* acShowDatumPlane;
		QAction* acHideDatumPlane;

		QAction* acSelectNeutral;
		QAction* acSelectVertex;
		QAction* acSelectEdge;
		QAction* acSelectFace;
		QAction* acSelectSolid;*/

		QMap<QWidget*, Model*> windowModelMap;

		ChildWindow* currentChild;

		PropertiesWidget* propertiesWidget;

		Controller* controller;
	};

}

#endif // MAIN_WINDOW_HEADER

