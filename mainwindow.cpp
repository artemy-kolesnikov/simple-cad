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

#include <AIS_SequenceOfInteractive.hxx>

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
	mainToolBar->addAction(acRectangle);

	acFunction = new QAction(tr("Функция"), this);
	mainToolBar->addAction(acFunction);

	mainToolBar->addSeparator();

	acStamp = new QAction(tr("Выдавливание"), this);
	mainToolBar->addAction(acStamp);

	acRotation = new QAction(tr("Вращение"), this);
	mainToolBar->addAction(acRotation);

	acAddition = new QAction(tr("Сложение"), this);
	mainToolBar->addAction(acAddition);

	acSubstract = new QAction(tr("Вычитание"), this);
	mainToolBar->addAction(acSubstract);
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
	ChildWindow* window = dynamic_cast<ChildWindow*>(mdiArea->activeSubWindow());
	if (!window)
		return;

	window->getController()->setMaterial(material);
}

void MainWindow::setShadded(bool shadded)
{
	ChildWindow* window = dynamic_cast<ChildWindow*>(mdiArea->activeSubWindow());
	if (!window)
		return;

	window->getController()->setShadded(shadded);
}

void MainWindow::viewSelectionChanged()
{
	ChildWindow* window = dynamic_cast<ChildWindow*>(mdiArea->activeSubWindow());
	if (!window)
		return;

	Model* model = window->getView()->getModel();

	boost::shared_ptr<AIS_SequenceOfInteractive> shapes = model->getSelectedShapes();

	Handle(AIS_Shape) shape;

	if (shapes->Length() == 1)
		shape = Handle(AIS_Shape)::DownCast(shapes->Value(1));

	propertiesWidget->setShape(shape);
}

