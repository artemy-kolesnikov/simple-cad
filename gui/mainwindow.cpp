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

#include "mainwindow.h"

#include <QActionGroup>
#include <QApplication>
#include <QDebug>
#include <QDir>
#include <QDockWidget>
#include <QFileDialog>
#include <QInputDialog>
#include <QMdiArea>
#include <QMdiSubWindow>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QSize>
#include <QToolBar>
#include <QTreeView>
#include <QVBoxLayout>

#include <AIS_SequenceOfInteractive.hxx>
#include <gp_Pnt.hxx>

#include <boost/shared_ptr.hpp>

#include "childwindow.h"
#include "model.h"
#include "controller.h"
#include "error.h"
#include "view.h"
#include "propertieswidget.h"
#include "positionsettingswidget.h"
#include "widgetdialog.h"
#include <exception.h>

#include <createcommand.h>

namespace Gui
{

	MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent)
	{
		controller = new Controller();

		createUI();
		createDockWidget();
		createMenu();
		createCommonActions();
		createOperationActions();
		createViewActions();

		currentChild = 0;
	}

	void MainWindow::openFiles(QStringList& files)
	{
		QString fileName;
		foreach(fileName, files)
		{
			ChildWindow* window = newChildWindow();
			//window->getController()->loadModel(fileName);
		}
	}

	void MainWindow::createUI()
	{
		resize(QSize(800, 600));

		mdiArea = new QMdiArea(this);
		setCentralWidget(mdiArea);
		//mdiArea->setViewMode(QMdiArea::TabbedView);
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

		QWidget* shapesWidget = new QWidget(shapesDock);
		QVBoxLayout* shapesLayout = new QVBoxLayout(shapesWidget);
		shapesDock->setWidget(shapesWidget);

		shapesTreeView = new QTreeView(shapesDock);
		shapesLayout->addWidget(shapesTreeView);

		propertiesWidget = new PropertiesWidget(this);
		shapesLayout->addWidget(propertiesWidget);

		connect(propertiesWidget, SIGNAL(shadedChanged(bool)),
			this, SLOT(setShadded(bool)));
	}

	void MainWindow::createMenu()
	{
		QMenuBar* mainMenuBar = new QMenuBar(this);
		setMenuBar(mainMenuBar);

		fileMenu = new QMenu(mainMenuBar);
		fileMenu->setTitle(tr("Файл"));
		mainMenuBar->addMenu(fileMenu);

		actionMenu = new QMenu(mainMenuBar);
		actionMenu->setTitle(tr("Действия"));
		mainMenuBar->addMenu(actionMenu);

		planeMenu = new QMenu(mainMenuBar);
		planeMenu->setTitle(tr("Плоскость"));
		actionMenu->addMenu(planeMenu);

		/*acSetDatumPlane = new QAction(tr("Установить плоскость"), this);
		planeMenu->addAction(acSetDatumPlane);
		connect(acSetDatumPlane, SIGNAL(triggered()), this, SLOT(setDatumPlane()));

		acShowDatumPlane = new QAction(tr("Показать плоскость"), this);
		planeMenu->addAction(acShowDatumPlane);
		connect(acShowDatumPlane, SIGNAL(triggered()), this, SLOT(showDatumPlane()));

		acHideDatumPlane = new QAction(tr("Спрятать плоскость"), this);
		planeMenu->addAction(acHideDatumPlane);
		connect(acHideDatumPlane, SIGNAL(triggered()), this, SLOT(hideDatumPlane()));

		actionMenu->addSeparator();*/

		/*QMenu* selectionMenu = new QMenu(mainMenuBar);
		selectionMenu->setTitle(tr("Выбор"));
		actionMenu->addMenu(selectionMenu);

		QActionGroup* selectionGroup = new QActionGroup(this);

		acSelectNeutral = new QAction(tr("Нейтральный"), this);
		acSelectNeutral->setCheckable(true);
		selectionMenu->addAction(acSelectNeutral);
		connect(acSelectNeutral, SIGNAL(triggered()), this, SLOT(selectNeutral()));
		selectionGroup->addAction(acSelectNeutral);

		acSelectVertex = new QAction(tr("Вершина"), this);
		acSelectVertex->setCheckable(true);
		selectionMenu->addAction(acSelectVertex);
		connect(acSelectVertex, SIGNAL(triggered()), this, SLOT(selectVertex()));
		selectionGroup->addAction(acSelectVertex);

		acSelectEdge = new QAction(tr("Грань"), this);
		acSelectEdge->setCheckable(true);
		selectionMenu->addAction(acSelectEdge);
		connect(acSelectEdge, SIGNAL(triggered()), this, SLOT(selectEdge()));
		selectionGroup->addAction(acSelectEdge);

		acSelectFace = new QAction(tr("Поверхность"), this);
		acSelectFace->setCheckable(true);
		selectionMenu->addAction(acSelectFace);
		connect(acSelectFace, SIGNAL(triggered()), this, SLOT(selectFace()));
		selectionGroup->addAction(acSelectFace);

		acSelectSolid = new QAction(tr("Объект"), this);
		acSelectSolid->setCheckable(true);
		selectionMenu->addAction(acSelectSolid);
		connect(acSelectSolid, SIGNAL(triggered()), this, SLOT(selectSolid()));
		selectionGroup->addAction(acSelectSolid);*/

		viewMenu = new QMenu(mainMenuBar);
		viewMenu->setTitle(tr("Вид"));
		mainMenuBar->addMenu(viewMenu);

		helpMenu = new QMenu(mainMenuBar);
		helpMenu->setTitle(tr("Помощь"));
		mainMenuBar->addMenu(helpMenu);

		creationMenu = new QMenu(mainMenuBar);
		creationMenu->setTitle(tr("Создать"));
		actionMenu->addMenu(creationMenu);

		operationMenu = new QMenu(mainMenuBar);
		operationMenu->setTitle(tr("Операции"));
		actionMenu->addMenu(operationMenu);

		mainToolBar = new QToolBar(this);
		addToolBar(mainToolBar);
	}

	void MainWindow::createCommonActions()
	{
		QAction* acExit = new QAction(tr("Выход"), this);
		connect(acExit, SIGNAL(triggered(bool)), qApp, SLOT(quit()));

		QAction* acOpenModel = new QAction(tr("Открыть..."), this);
		connect(acOpenModel, SIGNAL(triggered(bool)), this, SLOT(openModel()));

		QAction* acNewModel = new QAction(tr("Новый"), this);
		connect(acNewModel, SIGNAL(triggered(bool)), this, SLOT(newChildWindow()));

		fileMenu->addAction(acNewModel);
		fileMenu->addAction(acOpenModel);
		fileMenu->addSeparator();
		fileMenu->addAction(acExit);

		QAction* acAbout = new QAction(tr("О программе..."), this);
		connect(acAbout, SIGNAL(triggered(bool)), this, SLOT(about()));
		helpMenu->addAction(acAbout);
	}

	void MainWindow::createOperationActions()
	{
		QAction* acBox = new QAction(tr("Box"), this);
		connect(acBox, SIGNAL(triggered()), this, SLOT(createBox()));
		creationMenu->addAction(acBox);

		QAction* acCylinder = new QAction(tr("Cylinder"), this);
		connect(acCylinder, SIGNAL(triggered()), this, SLOT(createCylinder()));
		creationMenu->addAction(acCylinder);

		QAction* acSphere = new QAction(tr("Sphere"), this);
		connect(acSphere, SIGNAL(triggered()), this, SLOT(createSphere()));
		creationMenu->addAction(acSphere);

		QAction* acCone = new QAction(tr("Cone"), this);
		connect(acCone, SIGNAL(triggered()), this, SLOT(createCone()));
		creationMenu->addAction(acCone);

		QAction* acTorus = new QAction(tr("Torus"), this);
		connect(acTorus, SIGNAL(triggered()), this, SLOT(createTorus()));
		creationMenu->addAction(acTorus);

		QAction* acPlane = new QAction(tr("Plane"), this);
		connect(acPlane, SIGNAL(triggered()), this, SLOT(createPlane()));
		creationMenu->addAction(acPlane);

		QAction* acEllipsoid = new QAction(tr("Ellipsoid"), this);
		connect(acEllipsoid, SIGNAL(triggered()), this, SLOT(createEllipsoid()));
		creationMenu->addAction(acEllipsoid);
	}

	void MainWindow::createViewActions()
	{
		QActionGroup* viewGroup = new QActionGroup(this);

		QAction* acViewFront = new QAction(tr("Спереди"), this);
		acViewFront->setCheckable(true);
		viewMenu->addAction(acViewFront);
		connect(acViewFront, SIGNAL(triggered()), this, SLOT(viewFront()));
		viewGroup->addAction(acViewFront);

		QAction* acViewBack = new QAction(tr("Сзади"), this);
		acViewBack->setCheckable(true);
		viewMenu->addAction(acViewBack);
		connect(acViewBack, SIGNAL(triggered()), this, SLOT(viewBack()));
		viewGroup->addAction(acViewBack);

		QAction* acViewTop = new QAction(tr("Сверху"), this);
		acViewTop->setCheckable(true);
		viewMenu->addAction(acViewTop);
		connect(acViewTop, SIGNAL(triggered()), this, SLOT(viewTop()));
		viewGroup->addAction(acViewTop);

		QAction* acViewBottom = new QAction(tr("Снизу"), this);
		acViewBottom->setCheckable(true);
		viewMenu->addAction(acViewBottom);
		connect(acViewBottom, SIGNAL(triggered()), this, SLOT(viewBottom()));
		viewGroup->addAction(acViewBottom);

		QAction* acViewLeft = new QAction(tr("Слева"), this);
		acViewLeft->setCheckable(true);
		viewMenu->addAction(acViewLeft);
		connect(acViewLeft, SIGNAL(triggered()), this, SLOT(viewLeft()));
		viewGroup->addAction(acViewLeft);

		QAction* acViewRight = new QAction(tr("Справа"), this);
		acViewRight->setCheckable(true);
		viewMenu->addAction(acViewRight);
		connect(acViewRight, SIGNAL(triggered()), this, SLOT(viewRight()));
		viewGroup->addAction(acViewRight);

		QAction* acViewDatumPlane = new QAction(tr("По плоскости"), this);
		acViewDatumPlane->setCheckable(true);
		viewMenu->addAction(acViewDatumPlane);
		connect(acViewDatumPlane, SIGNAL(triggered()), this, SLOT(viewDatumPlane()));
		viewGroup->addAction(acViewDatumPlane);
	}

	ChildWindow* MainWindow::currentChildWindow() const
	{
		qDebug() << mdiArea->activeSubWindow();
		ChildWindow* window = dynamic_cast<ChildWindow*>(mdiArea->activeSubWindow());
		assert(window);
		return window;
	}

	Model* MainWindow::currentModel() const
	{
		ChildWindow* window = currentChildWindow();
		return window->getView()->getModel();
	}

	void MainWindow::shapesVisChanged(bool)
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
		catch(Common::FileException& err)
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
		//shapesTreeView->setModel(currentChild->getShapeModel());

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

	void MainWindow::setShadded(bool shadded)
	{
		ChildWindow* window = currentChildWindow();

		//window->getController()->setShadded(shadded);
	}

	void MainWindow::viewSelectionChanged()
	{
		/*ChildWindow* window = currentChildWindow();
		if (!window)
			return;

		Model* model = window->getView()->getModel();

		boost::shared_ptr<AIS_SequenceOfInteractive> shapes = model->getCurrentShapes();

		Handle(AIS_Shape) shape;

		if (shapes->Length() == 1)
			shape = Handle(AIS_Shape)::DownCast(shapes->Value(1));

		propertiesWidget->setShape(shape);*/
	}

	void MainWindow::makePrism()
	{
		/*Model* model = currentModel();
		if (!model)
			return;

		boost::shared_ptr<AIS_SequenceOfInteractive> shapes = model->getCurrentShapes();
		if (shapes->Length() == 1)
		{
			Handle(AIS_Shape) shape = Handle(AIS_Shape)::DownCast(shapes->Value(1));
			model->makePrism(shape, 150);
		}*/
	}

	void MainWindow::setDatumPlane()
	{
		Model* model = currentModel();
		if (!model)
			return;

		PositionSettingsWidget* planeWidget = new PositionSettingsWidget(this);

		gp_Pnt pt;
		gp_Dir dir;
		//model->getDatumPlane(pt, dir);
		planeWidget->setPoint(pt);	
		planeWidget->setDirection(dir);

		WidgetDialog* dlg = new WidgetDialog(this);
		dlg->setCentralWidget(planeWidget);
		if (dlg->exec() == QDialog::Accepted)
		{
			gp_Pnt pt = planeWidget->getPoint();
			gp_Dir dir = planeWidget->getDirection();

			//model->setDatumPlane(pt, dir);
			//model->showDatumPlane();
		}
	}

	void MainWindow::showDatumPlane()
	{	
		Model* model = currentModel();
		if (!model)
			return;

		//model->showDatumPlane();
	}

	void MainWindow::hideDatumPlane()
	{
		Model* model = currentModel();
		if (!model)
			return;

		//model->hideDatumPlane();
	}

	void MainWindow::viewFront()
	{
		ChildWindow* window = currentChildWindow();
		window->getView()->viewFront();
	}

	void MainWindow::viewBack()
	{
		ChildWindow* window = currentChildWindow();
		window->getView()->viewBack();
	}

	void MainWindow::viewTop()
	{
		ChildWindow* window = currentChildWindow();
		window->getView()->viewTop();
	}

	void MainWindow::viewBottom()
	{
		ChildWindow* window = currentChildWindow();
		window->getView()->viewBottom();
	}

	void MainWindow::viewLeft()
	{
		ChildWindow* window = currentChildWindow();
		window->getView()->viewLeft();
	}

	void MainWindow::viewRight()
	{
		ChildWindow* window = currentChildWindow();
		window->getView()->viewRight();
	}

	void MainWindow::viewDatumPlane()
	{
		ChildWindow* window = currentChildWindow();
		window->getView()->viewDatumPlane();
	}

	void MainWindow::selectNeutral()
	{
		Model* model = currentModel();
		//if (model)
			//model->selectNeutral();
	}

	void MainWindow::selectVertex()
	{
		Model* model = currentModel();
		//if (model)
			//model->selectVertex();
	}

	void MainWindow::selectEdge()
	{
		Model* model = currentModel();
		//if (model)
			//model->selectEdge();
	}

	void MainWindow::selectFace()
	{
		Model* model = currentModel();
		//if (model)
			//model->selectFace();
	}

	void MainWindow::selectSolid()
	{
		Model* model = currentModel();
		//if (model)
			//model->selectSolid();
	}

	void MainWindow::createBox()
	{
		Model* model = currentModel();
		CreateCommand* cmd = new CreateCommand(model,
			CreateCommand::Box);
		controller->execCommand(cmd);
	}

	void MainWindow::createCylinder()
	{
		Model* model = currentModel();
		CreateCommand* cmd = new CreateCommand(model,
			CreateCommand::Cylinder);
		controller->execCommand(cmd);
	}

	void MainWindow::createSphere()
	{
		Model* model = currentModel();
		CreateCommand* cmd = new CreateCommand(model,
			CreateCommand::Sphere);
		controller->execCommand(cmd);
	}

	void MainWindow::createCone()
	{
		Model* model = currentModel();
		CreateCommand* cmd = new CreateCommand(model,
			CreateCommand::Cone);
		controller->execCommand(cmd);
	}

	void MainWindow::createTorus()
	{
		Model* model = currentModel();
		CreateCommand* cmd = new CreateCommand(model,
			CreateCommand::Torus);
		controller->execCommand(cmd);
	}

	void MainWindow::createPlane()
	{
		Model* model = currentModel();
		CreateCommand* cmd = new CreateCommand(model,
			CreateCommand::Plane);
		controller->execCommand(cmd);
	}

	void MainWindow::createEllipsoid()
	{
		Model* model = currentModel();
		CreateCommand* cmd = new CreateCommand(model,
			CreateCommand::Ellipsoid);
		controller->execCommand(cmd);
	}
}

