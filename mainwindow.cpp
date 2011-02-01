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

#include "mainwindow.h"

#include <QSize>
#include <QMdiArea>
#include <QDockWidget>
#include <QMenuBar>
#include <QMenu>
#include <QApplication>
#include <QTreeView>
#include <QFileDialog>
#include <QDir>
#include <QMessageBox>
#include <QToolBar>
#include <QMdiSubWindow>
#include <QInputDialog>
#include <QDebug>
#include <QActionGroup>

#include <AIS_SequenceOfInteractive.hxx>
#include <gp_Pnt.hxx>

#include <boost/shared_ptr.hpp>

#include "childwindow.h"
#include "model.h"
#include "controller.h"
#include "error.h"
#include "view.h"
#include "qshapemodel.h"
#include "propertieswidget.h"

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent)
{
	createUI();
	createDockWidget();
	createMenuAndActions();

	currentChild = 0;
}

void MainWindow::openFiles(QStringList& files)
{
	QString fileName;
	foreach(fileName, files)
	{
		ChildWindow* window = newChildWindow();
		window->getController()->loadModel(fileName);
	}
}

void MainWindow::createUI()
{
	resize(QSize(800, 600));

	mdiArea = new QMdiArea(this);
	setCentralWidget(mdiArea);
	connect(mdiArea, SIGNAL(subWindowActivated(QMdiSubWindow*)),
		this, SLOT(childWindowActivated(QMdiSubWindow*)));
}

void MainWindow::createDockWidget()
{
	shapesDock = new QDockWidget(tr("Shapes"), this);
	shapesDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
	addDockWidget(Qt::LeftDockWidgetArea, shapesDock);

	connect(shapesDock, SIGNAL(visibilityChanged(bool)), this,
		SLOT(shapesVisChanged(bool)));

	shapesTreeView = new QTreeView(shapesDock);
	shapesDock->setWidget(shapesTreeView);

	propertiesDock = new QDockWidget(tr("Properties"), this);
	propertiesDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
	addDockWidget(Qt::RightDockWidgetArea, propertiesDock);

	connect(propertiesDock, SIGNAL(visibilityChanged(bool)), this,
		SLOT(propertiesVisChanged(bool)));

	propertiesWidget = new PropertiesWidget(this);
	propertiesDock->setWidget(propertiesWidget);

	connect(propertiesWidget, SIGNAL(materialChanged(Graphic3d_NameOfMaterial)),
		this, SLOT(setMaterial(Graphic3d_NameOfMaterial)));
	connect(propertiesWidget, SIGNAL(shadedChanged(bool)),
		this, SLOT(setShadded(bool)));
}

void MainWindow::createMenuAndActions()
{
	QMenuBar* mainMenuBar = new QMenuBar(this);
	setMenuBar(mainMenuBar);

	acExit = new QAction(tr("Выход"), this);
	connect(acExit, SIGNAL(triggered(bool)), qApp, SLOT(quit()));

	acOpenModel = new QAction(tr("Открыть..."), this);
	connect(acOpenModel, SIGNAL(triggered(bool)), this, SLOT(openModel()));

	acNewModel = new QAction(tr("Новый"), this);
	connect(acNewModel, SIGNAL(triggered(bool)), this, SLOT(newChildWindow()));

	QMenu* fileMenu = new QMenu(mainMenuBar);
	fileMenu->setTitle(tr("Файл"));
	mainMenuBar->addMenu(fileMenu);

	fileMenu->addAction(acNewModel);
	fileMenu->addAction(acOpenModel);
	fileMenu->addSeparator();
	fileMenu->addAction(acExit);

	QMenu* actionMenu = new QMenu(mainMenuBar);
	actionMenu->setTitle(tr("Действия"));
	mainMenuBar->addMenu(actionMenu);

	acSetDatumPlane = new QAction(tr("Установить плоскость"), this);
	actionMenu->addAction(acSetDatumPlane);
	connect(acSetDatumPlane, SIGNAL(triggered()), this, SLOT(setDatumPlane()));

	actionMenu->addSeparator();

	QActionGroup* selectionGroup = new QActionGroup(this);

	acSelectNeutral = new QAction(tr("Нейтральный"), this);
	acSelectNeutral->setCheckable(true);
	actionMenu->addAction(acSelectNeutral);
	connect(acSelectNeutral, SIGNAL(triggered()), this, SLOT(selectNeutral()));
	selectionGroup->addAction(acSelectNeutral);

	acSelectVertex = new QAction(tr("Вершина"), this);
	acSelectVertex->setCheckable(true);
	actionMenu->addAction(acSelectVertex);
	connect(acSelectVertex, SIGNAL(triggered()), this, SLOT(selectVertex()));
	selectionGroup->addAction(acSelectVertex);

	acSelectEdge = new QAction(tr("Грань"), this);
	acSelectEdge->setCheckable(true);
	actionMenu->addAction(acSelectEdge);
	connect(acSelectEdge, SIGNAL(triggered()), this, SLOT(selectEdge()));
	selectionGroup->addAction(acSelectEdge);

	acSelectFace = new QAction(tr("Поверхность"), this);
	acSelectFace->setCheckable(true);
	actionMenu->addAction(acSelectFace);
	connect(acSelectFace, SIGNAL(triggered()), this, SLOT(selectFace()));
	selectionGroup->addAction(acSelectFace);

	acSelectSolid = new QAction(tr("Объект"), this);
	acSelectSolid->setCheckable(true);
	actionMenu->addAction(acSelectSolid);
	connect(acSelectSolid, SIGNAL(triggered()), this, SLOT(selectSolid()));
	selectionGroup->addAction(acSelectSolid);

	QMenu* viewMenu = new QMenu(mainMenuBar);
	viewMenu->setTitle(tr("Вид"));
	mainMenuBar->addMenu(viewMenu);

	QActionGroup* viewGroup = new QActionGroup(this);

	acViewFront = new QAction(tr("Спереди"), this);
	acViewFront->setCheckable(true);
	viewMenu->addAction(acViewFront);
	connect(acViewFront, SIGNAL(triggered()), this, SLOT(viewFront()));
	viewGroup->addAction(acViewFront);

	acViewBack = new QAction(tr("Сзади"), this);
	acViewBack->setCheckable(true);
	viewMenu->addAction(acViewBack);
	connect(acViewBack, SIGNAL(triggered()), this, SLOT(viewBack()));
	viewGroup->addAction(acViewBack);

	acViewTop = new QAction(tr("Сверху"), this);
	acViewTop->setCheckable(true);
	viewMenu->addAction(acViewTop);
	connect(acViewTop, SIGNAL(triggered()), this, SLOT(viewTop()));
	viewGroup->addAction(acViewTop);

	acViewBottom = new QAction(tr("Снизу"), this);
	acViewBottom->setCheckable(true);
	viewMenu->addAction(acViewBottom);
	connect(acViewBottom, SIGNAL(triggered()), this, SLOT(viewBottom()));
	viewGroup->addAction(acViewBottom);

	acViewLeft = new QAction(tr("Слева"), this);
	acViewLeft->setCheckable(true);
	viewMenu->addAction(acViewLeft);
	connect(acViewLeft, SIGNAL(triggered()), this, SLOT(viewLeft()));
	viewGroup->addAction(acViewLeft);

	acViewRight = new QAction(tr("Справа"), this);
	acViewRight->setCheckable(true);
	viewMenu->addAction(acViewRight);
	connect(acViewRight, SIGNAL(triggered()), this, SLOT(viewRight()));
	viewGroup->addAction(acViewRight);

	acViewDatumPlane = new QAction(tr("По плоскости"), this);
	acViewDatumPlane->setCheckable(true);
	viewMenu->addAction(acViewDatumPlane);
	connect(acViewDatumPlane, SIGNAL(triggered()), this, SLOT(viewDatumPlane()));
	viewGroup->addAction(acViewDatumPlane);

	acAbout = new QAction(tr("О программе..."), this);
	connect(acAbout, SIGNAL(triggered(bool)), this, SLOT(about()));

	QMenu* helpMenu = new QMenu(mainMenuBar);
	helpMenu->setTitle(tr("Помощь"));
	mainMenuBar->addMenu(helpMenu);

	helpMenu->addAction(acAbout);

	mainToolBar = new QToolBar(this);
	addToolBar(mainToolBar);

	acLine = new QAction(tr("Линия"), this);
	mainToolBar->addAction(acLine);

	acCurve = new QAction(tr("Ломаная линия"), this);
	mainToolBar->addAction(acCurve);

	acEllipse = new QAction(tr("Эллипс"), this);
	mainToolBar->addAction(acEllipse);

	acRectangle = new QAction(tr("Прямоугольник"), this);
	connect(acRectangle, SIGNAL(triggered()), this, SLOT(createRectangle()));
	mainToolBar->addAction(acRectangle);

	acFunction = new QAction(tr("Функция"), this);
	mainToolBar->addAction(acFunction);

	mainToolBar->addSeparator(); 
	acStamp = new QAction(tr("Выдавливание"), this);
	connect(acStamp, SIGNAL(triggered()), this, SLOT(makePrism()));
	mainToolBar->addAction(acStamp);

	acRotation = new QAction(tr("Вращение"), this);
	mainToolBar->addAction(acRotation);

	acAddition = new QAction(tr("Сложение"), this);
	mainToolBar->addAction(acAddition);

	acSubstract = new QAction(tr("Вычитание"), this);
	mainToolBar->addAction(acSubstract);
}

ChildWindow* MainWindow::currentChildWindow() const
{
	ChildWindow* window = dynamic_cast<ChildWindow*>(mdiArea->activeSubWindow());
	return window;
}

Model* MainWindow::currentModel() const
{
	ChildWindow* window = currentChildWindow();
	if (window)
		return window->getView()->getModel();

	return 0;
}

void MainWindow::shapesVisChanged(bool)
{
}

void MainWindow::propertiesVisChanged(bool)
{
}

void MainWindow::about()
{
}

void MainWindow::openModel()
{
	QString currentDir = QDir::currentPath();

	QStringList files = QFileDialog::getOpenFileNames(
                         this,
                         tr("Выберите один или несколько файлов"),
                         currentDir,
                         "Model files (*.csfdb *.iges *.brep *.rle *.step *.stl)\n"
						 "Images (*.xwd *.bmp *.png *.jpeg *.jpg *.gif)\n"
						 "All files(*.*)");

	try
	{
		openFiles(files);
	}
	catch(FileError& err)
	{
		QMessageBox::critical(this, tr("Ошибка"), err.what());
		return;
	}
}

ChildWindow* MainWindow::newChildWindow()
{
	Model* model = new Model(this);

	ChildWindow* window = new ChildWindow(model, this);
	mdiArea->addSubWindow(window);

	windowModelMap.insert(window, model);

	window->show();
	window->setWindowState(Qt::WindowMaximized);
	window->setAttribute(Qt::WA_DeleteOnClose);

	return window;
}

void MainWindow::childWindowActivated(QMdiSubWindow* window)
{
	if (!window)
		return;

	if (currentChild)
	{
		disconnect(currentChild->getView()->getModel(), SIGNAL(changed()),
			this, SLOT(modelChanged()));

		disconnect(currentChild, SIGNAL(selectionChanged()),
			this, SLOT(viewSelectionChanged()));
	}

	currentChild = qobject_cast<ChildWindow*>(window);
	shapesTreeView->setModel(currentChild->getShapeModel());

	connect(currentChild->getView()->getModel(), SIGNAL(changed()),
		this, SLOT(modelChanged()));

	connect(currentChild, SIGNAL(selectionChanged()),
		this, SLOT(viewSelectionChanged()));
}

void MainWindow::modelChanged()
{
	Model* model = static_cast<Model*>(sender());

	/*Handle(TopTools_HSequenceOfShape) shapes = model->getShapes();

	for (int i = 1; i <= shapes->Length(); ++i)
	{
	}*/
}

void MainWindow::setMaterial(Graphic3d_NameOfMaterial material)
{
	ChildWindow* window = currentChildWindow();
	if (!window)
		return;

	window->getController()->setMaterial(material);
}

void MainWindow::setShadded(bool shadded)
{
	ChildWindow* window = currentChildWindow();
	if (!window)
		return;

	window->getController()->setShadded(shadded);
}

void MainWindow::viewSelectionChanged()
{
	ChildWindow* window = currentChildWindow();
	if (!window)
		return;

	Model* model = window->getView()->getModel();

	boost::shared_ptr<AIS_SequenceOfInteractive> shapes = model->getSelectedShapes();

	Handle(AIS_Shape) shape;

	if (shapes->Length() == 1)
		shape = Handle(AIS_Shape)::DownCast(shapes->Value(1));

	propertiesWidget->setShape(shape);
}

void MainWindow::createRectangle()
{
	ChildWindow* window = currentChildWindow();
	if (!window)
		return;

	gp_Pnt pt(0, 0, 0);
	window->getController()->createRectangle(pt, 100, 100);
}

void MainWindow::makePrism()
{
	Model* model = currentModel();
	if (!model)
		return;

	boost::shared_ptr<AIS_SequenceOfInteractive> shapes = model->getSelectedShapes();
	if (shapes->Length() == 1)
	{
		Handle(AIS_Shape) shape = Handle(AIS_Shape)::DownCast(shapes->Value(1));
		model->makePrism(shape, 150);
	}
}

void MainWindow::setDatumPlane()
{
}

void MainWindow::viewFront()
{
	ChildWindow* window = currentChildWindow();
	if (!window)
		return;

	window->getView()->viewFront();
}

void MainWindow::viewBack()
{
	ChildWindow* window = currentChildWindow();
	if (!window)
		return;

	window->getView()->viewBack();
}

void MainWindow::viewTop()
{
	ChildWindow* window = currentChildWindow();
	if (!window)
		return;

	window->getView()->viewTop();
}

void MainWindow::viewBottom()
{
	ChildWindow* window = currentChildWindow();
	if (!window)
		return;

	window->getView()->viewBottom();
}

void MainWindow::viewLeft()
{
	ChildWindow* window = currentChildWindow();
	if (!window)
		return;

	window->getView()->viewLeft();
}

void MainWindow::viewRight()
{
	ChildWindow* window = currentChildWindow();
	if (!window)
		return;

	window->getView()->viewRight();
}

void MainWindow::viewDatumPlane()
{
	ChildWindow* window = currentChildWindow();
	if (!window)
		return;

	window->getView()->viewDatumPlane();
}

void MainWindow::selectNeutral()
{
	Model* model = currentModel();
	if (model)
		model->selectNeutral();
}

void MainWindow::selectVertex()
{
	Model* model = currentModel();
	if (model)
		model->selectVertex();
}

void MainWindow::selectEdge()
{
	Model* model = currentModel();
	if (model)
		model->selectEdge();
}

void MainWindow::selectFace()
{
	Model* model = currentModel();
	if (model)
		model->selectFace();
}

void MainWindow::selectSolid()
{
	Model* model = currentModel();
	if (model)
		model->selectSolid();
}

