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
#include <QStringList>

#include <Graphic3d_NameOfMaterial.hxx>

class QMdiArea;
class QDockWidget;
class QMenuBar;
class QAction;
class QTreeView;
class Model;
class QToolBar;
class QMdiSubWindow;

class ChildWindow;
class PropertiesWidget;

class MainWindow : public QMainWindow
{
	Q_OBJECT
public:
	MainWindow(QWidget* parent = 0);

	void openFiles(QStringList& files);

private Q_SLOTS:
	void shapesVisChanged(bool);
	void about();
	void openModel();
	ChildWindow* newChildWindow();
	void childWindowActivated(QMdiSubWindow* window);
	void modelChanged();
	void setMaterial(Graphic3d_NameOfMaterial material);
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

	void removeShape();

	void shapesFusion();
	void shapesCommon();
	void shapesCut();

private:
	void createUI();
	void createDockWidget();
	void createMenuAndActions();

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

	QAction* acExit;
	QAction* acAbout;
	QAction* acOpenModel;
	QAction* acNewModel;

	QAction* acBox;
	QAction* acCylinder;
	QAction* acSphere;
	QAction* acCone;
	QAction* acTorus;
	QAction* acPlane;
	QAction* acEllipsoid;

	/*QAction* acStamp;
	QAction* acRotation;
	QAction* acAddition;
	QAction* acSubstract;*/

	QAction* acFusion;
	QAction* acCommon;
	QAction* acCut;

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
	QAction* acSelectSolid;

	QAction* acRemoveShape;

	QMap<QWidget*, Model*> windowModelMap;

	ChildWindow* currentChild;

	PropertiesWidget* propertiesWidget;
};

#endif // MAIN_WINDOW_HEADER
