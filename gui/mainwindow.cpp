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

#include <gp_Pnt.hxx>
#include <Inventor/SbPlane.h>
#include <cassert>

#include "childwindow.h"
#include "model.h"
#include "controller.h"
#include "error.h"
#include "view.h"
#include "propertieswidget.h"
#include "positionsettingswidget.h"
#include "widgetdialog.h"
#include <exception.h>

#include <createprimitivecommand.h>
#include <createsketchcommand.h>
#include <removecommand.h>
#include <commandmessage.h>
#include <sendmessagecommand.h>
#include <sketchcommon.h>
#include <shapemodel.h>
#include <boolcommand.h>
#include <filletcommand.h>
#include <version.h>
#include <actionlist.h>

namespace Gui
{

	MainWindow::MainWindow(QWidget* parent) :
		QMainWindow(parent), commandWidget(0), lastCommandType(CommandNone)
	{
		controller = new Controller();

		createUI();
		createDockWidget();
		createMenu();
		createCommonActions();
		createOperationActions();
		createViewActions();
		//createSketcherAction();

		disableCreateActions();
		disableViewActions();
		disableSketch();
		disableModOperations();

		currentChild = 0;
	}

	void MainWindow::showEvent(QShowEvent* event)
	{
		//createSketch();

		QWidget::showEvent(event);
	}

	void MainWindow::openFiles(QStringList& files)
	{
		QString fileName;
		foreach(fileName, files)
		{
			ChildWindow* window = newChildWindow();
			Model& model = currentModel();
			model.load(fileName);
		}
	}

	void MainWindow::setCommandWidget(QWidget* widget, MainWindow::CommandType type)
	{
		if (type == CommandNone)
			return;

		switch(type)
		{
			case CommandMod:
				disableCreateActions();
				disableSketch();
				disableModOperations();
				break;
		}

		commandWidget = widget;
		commandDock->setWidget(commandWidget);
		commandDock->setVisible(true);
		lastCommandType = type;
	}

	void MainWindow::removeCommandWidget()
	{
		if (!commandWidget)
			return;

		switch(lastCommandType)
		{
			case CommandMod:
				enableCreateActions();
				enableSketch();
				enableModOperations();
				break;
		}

		commandWidget = 0;
		commandDock->setWidget(0);
		commandDock->setVisible(false);
		lastCommandType = CommandNone;
	}

	void MainWindow::createUI()
	{
		resize(QSize(800, 600));

		setWindowTitle(tr("Simple CAD"));

		mdiArea = new QMdiArea(this);
		setCentralWidget(mdiArea);
		//mdiArea->setViewMode(QMdiArea::TabbedView);
		connect(mdiArea, SIGNAL(subWindowActivated(QMdiSubWindow*)),
			this, SLOT(childWindowActivated(QMdiSubWindow*)));

		commandDock = new QDockWidget(tr("Команда"), this);
		commandDock->setVisible(false);
		commandDock->setFeatures(QDockWidget::NoDockWidgetFeatures);
		commandDock->setAllowedAreas(Qt::RightDockWidgetArea);
		addDockWidget(Qt::RightDockWidgetArea, commandDock);
	}

	void MainWindow::createDockWidget()
	{
		shapesDock = new QDockWidget(tr("Объекты"), this);
		shapesDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
		addDockWidget(Qt::LeftDockWidgetArea, shapesDock);

		connect(shapesDock, SIGNAL(visibilityChanged(bool)), this,
			SLOT(shapesVisChanged(bool)));

		QWidget* shapesWidget = new QWidget(shapesDock);
		QVBoxLayout* shapesLayout = new QVBoxLayout(shapesWidget);
		shapesDock->setWidget(shapesWidget);

		shapesTreeView = new QTreeView(shapesDock);
		shapesLayout->addWidget(shapesTreeView);
	}

	void MainWindow::createMenu()
	{
		mainMenuBar = new QMenuBar(this);
		setMenuBar(mainMenuBar);

		fileMenu = new QMenu(mainMenuBar);
		fileMenu->setTitle(tr("Файл"));
		mainMenuBar->addMenu(fileMenu);

		actionMenu = new QMenu(mainMenuBar);
		actionMenu->setTitle(tr("Действия"));
		mainMenuBar->addMenu(actionMenu);

		sketcherMenu = new QMenu(mainMenuBar);
		sketcherMenu->setTitle(tr("Эскиз"));
		//mainMenuBar->addMenu(sketcherMenu);

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
		acList3dPrimitive = new ActionList(this);
		acListSketchPrimitive = new ActionList(this);
		acListViewType = new ActionList(this);
		acListModOperations = new ActionList(this);

		QAction* acExit = new QAction(tr("Выход"), this);
		connect(acExit, SIGNAL(triggered(bool)), qApp, SLOT(quit()));

		QAction* acOpenModel = new QAction(QIcon(":/gui/icons/open_document.png"), tr("Открыть..."), this);
		connect(acOpenModel, SIGNAL(triggered(bool)), this, SLOT(openModel()));

		QAction* acNewModel = new QAction(QIcon(":/gui/icons/new_document.png"), tr("Новый"), this);
		connect(acNewModel, SIGNAL(triggered(bool)), this, SLOT(newChildWindow()));

		fileMenu->addAction(acNewModel);
		fileMenu->addAction(acOpenModel);
		fileMenu->addSeparator();
		fileMenu->addAction(acExit);

		mainToolBar->addAction(acNewModel);
		mainToolBar->addAction(acOpenModel);
		mainToolBar->addSeparator();

		QAction* acAbout = new QAction(tr("О программе..."), this);
		connect(acAbout, SIGNAL(triggered(bool)), this, SLOT(about()));
		helpMenu->addAction(acAbout);
	}

	void MainWindow::createOperationActions()
	{
		QMenu* primitivesMenu = new QMenu(mainMenuBar);
		primitivesMenu->setTitle(tr("Примитивы"));
		creationMenu->addMenu(primitivesMenu);

		QAction* acBox = new QAction(QIcon(":/gui/icons/box.png"), tr("Брусок"), this);
		connect(acBox, SIGNAL(triggered()), this, SLOT(createBox()));
		primitivesMenu->addAction(acBox);
		mainToolBar->addAction(acBox);
		acList3dPrimitive->addAction(acBox);

		QAction* acCylinder = new QAction(QIcon(":/gui/icons/cylinder.png"), tr("Цилиндр"), this);
		connect(acCylinder, SIGNAL(triggered()), this, SLOT(createCylinder()));
		primitivesMenu->addAction(acCylinder);
		mainToolBar->addAction(acCylinder);
		acList3dPrimitive->addAction(acCylinder);

		QAction* acSphere = new QAction(QIcon(":/gui/icons/sphere.png"), tr("Сфера"), this);
		connect(acSphere, SIGNAL(triggered()), this, SLOT(createSphere()));
		primitivesMenu->addAction(acSphere);
		mainToolBar->addAction(acSphere);
		acList3dPrimitive->addAction(acSphere);

		QAction* acCone = new QAction(QIcon(":/gui/icons/cone.png"), tr("Конус"), this);
		connect(acCone, SIGNAL(triggered()), this, SLOT(createCone()));
		primitivesMenu->addAction(acCone);
		mainToolBar->addAction(acCone);
		acList3dPrimitive->addAction(acCone);

		QAction* acTorus = new QAction(QIcon(":/gui/icons/torus.png"), tr("Тор"), this);
		connect(acTorus, SIGNAL(triggered()), this, SLOT(createTorus()));
		primitivesMenu->addAction(acTorus);
		mainToolBar->addAction(acTorus);
		acList3dPrimitive->addAction(acTorus);

		QAction* acPlane = new QAction(QIcon(":/gui/icons/plane.png"), tr("Плоскость"), this);
		connect(acPlane, SIGNAL(triggered()), this, SLOT(createPlane()));
		primitivesMenu->addAction(acPlane);
		mainToolBar->addAction(acPlane);
		acList3dPrimitive->addAction(acPlane);

		QAction* acEllipsoid = new QAction(QIcon(":/gui/icons/ellipsoid.png"), tr("Еллипсоид"), this);
		connect(acEllipsoid, SIGNAL(triggered()), this, SLOT(createEllipsoid()));
		primitivesMenu->addAction(acEllipsoid);
		mainToolBar->addAction(acEllipsoid);
		acList3dPrimitive->addAction(acEllipsoid);

		mainToolBar->addSeparator();

		QAction* acSketch = new QAction(tr("Эскиз"), this);
		connect(acSketch, SIGNAL(triggered()), this, SLOT(createSketch()));
		//creationMenu->addAction(acSketch);

		acRemove = new QAction(tr("Удалить"), this);
		acRemove->setShortcut(Qt::Key_Delete);
		connect(acRemove, SIGNAL(triggered()), this, SLOT(removeShape()));
		actionMenu->addAction(acRemove);
		acRemove->setEnabled(false);
		acListModOperations->addAction(acRemove);

		acMove = new QAction(tr("Режим перемещения"), this);
		connect(acMove, SIGNAL(triggered()), this, SLOT(moveShape()));
		actionMenu->addAction(acMove);
		acMove->setCheckable(true);
		acListModOperations->addAction(acMove);

		QAction* acFillet = new QAction(tr("Скругление"), this);
		connect(acFillet, SIGNAL(triggered()), this, SLOT(makeFillet()));
		operationMenu->addAction(acFillet);
		acListModOperations->addAction(acFillet);

		QAction* acBoolean = new QAction(tr("Булевы операции"), this);
		connect(acBoolean, SIGNAL(triggered()), this, SLOT(booleanOperation()));
		operationMenu->addAction(acBoolean);
		acListModOperations->addAction(acBoolean);
	}

	void MainWindow::createViewActions()
	{
		QActionGroup* viewGroup = new QActionGroup(this);

		QAction* acViewFront = new QAction(QIcon(":/gui/icons/view-front.png"), tr("Спереди"), this);
		acViewFront->setCheckable(true);
		viewMenu->addAction(acViewFront);
		connect(acViewFront, SIGNAL(triggered()), this, SLOT(viewFront()));
		viewGroup->addAction(acViewFront);
		mainToolBar->addAction(acViewFront);
		acListViewType->addAction(acViewFront);

		QAction* acViewBack = new QAction(QIcon(":/gui/icons/view-back.png"), tr("Сзади"), this);
		acViewBack->setCheckable(true);
		viewMenu->addAction(acViewBack);
		connect(acViewBack, SIGNAL(triggered()), this, SLOT(viewBack()));
		viewGroup->addAction(acViewBack);
		mainToolBar->addAction(acViewBack);
		acListViewType->addAction(acViewBack);

		QAction* acViewTop = new QAction(QIcon(":/gui/icons/view-top.png"), tr("Сверху"), this);
		acViewTop->setCheckable(true);
		viewMenu->addAction(acViewTop);
		connect(acViewTop, SIGNAL(triggered()), this, SLOT(viewTop()));
		viewGroup->addAction(acViewTop);
		mainToolBar->addAction(acViewTop);
		acListViewType->addAction(acViewTop);

		QAction* acViewBottom = new QAction(QIcon(":/gui/icons/view-bottom.png"), tr("Снизу"), this);
		acViewBottom->setCheckable(true);
		viewMenu->addAction(acViewBottom);
		connect(acViewBottom, SIGNAL(triggered()), this, SLOT(viewBottom()));
		viewGroup->addAction(acViewBottom);
		mainToolBar->addAction(acViewBottom);
		acListViewType->addAction(acViewBottom);

		QAction* acViewLeft = new QAction(QIcon(":/gui/icons/view-left.png"), tr("Слева"), this);
		acViewLeft->setCheckable(true);
		viewMenu->addAction(acViewLeft);
		connect(acViewLeft, SIGNAL(triggered()), this, SLOT(viewLeft()));
		viewGroup->addAction(acViewLeft);
		mainToolBar->addAction(acViewLeft);
		acListViewType->addAction(acViewLeft);

		QAction* acViewRight = new QAction(QIcon(":/gui/icons/view-right.png"), tr("Справа"), this);
		acViewRight->setCheckable(true);
		viewMenu->addAction(acViewRight);
		connect(acViewRight, SIGNAL(triggered()), this, SLOT(viewRight()));
		viewGroup->addAction(acViewRight);
		mainToolBar->addAction(acViewRight);
		acListViewType->addAction(acViewRight);

		QAction* acAxometric = new QAction(QIcon(":/gui/icons/view-axometric.png"), tr("Аксометрически"), this);
		acAxometric->setCheckable(true);
		viewMenu->addAction(acAxometric);
		connect(acAxometric, SIGNAL(triggered()), this, SLOT(viewAxometric()));
		viewGroup->addAction(acAxometric);
		mainToolBar->addAction(acAxometric);
		acListViewType->addAction(acAxometric);

		QAction* acViewAll = new QAction(QIcon(":/gui/icons/view-all.png"), tr("Смотреть всё"), this);
		viewMenu->addAction(acViewAll);
		connect(acViewAll, SIGNAL(triggered()), this, SLOT(viewAll()));
		acListViewType->addAction(acViewAll);

		mainToolBar->addSeparator();
	}

	void MainWindow::createSketcherAction()
	{
		QAction* acPolyline = new QAction(QIcon(":/gui/icons/polyline.png"), tr("Ломанаия линия"), this);
		connect(acPolyline, SIGNAL(triggered()), this, SLOT(sketchPolyline()));
		sketcherMenu->addAction(acPolyline);
		mainToolBar->addAction(acPolyline);
		acListSketchPrimitive->addAction(acPolyline);

		QAction* acRectangle = new QAction(QIcon(":/gui/icons/rectangle.png"), tr("Прямоугольник"), this);
		connect(acRectangle, SIGNAL(triggered()), this, SLOT(sketchRectangle()));
		sketcherMenu->addAction(acRectangle);
		mainToolBar->addAction(acRectangle);
		acListSketchPrimitive->addAction(acRectangle);

		QAction* acCircle = new QAction(QIcon(":/gui/icons/circle.png"), tr("Окружность"), this);
		connect(acCircle, SIGNAL(triggered()), this, SLOT(sketchCircle()));
		sketcherMenu->addAction(acCircle);
		mainToolBar->addAction(acCircle);
		acListSketchPrimitive->addAction(acCircle);

		QAction* acArc = new QAction(QIcon(":/gui/icons/arc.png"), tr("Дуга"), this);
		connect(acArc, SIGNAL(triggered()), this, SLOT(sketchArc()));
		sketcherMenu->addAction(acArc);
		mainToolBar->addAction(acArc);
		acListSketchPrimitive->addAction(acArc);

		QAction* acNurbs = new QAction(tr("NURBS кривая"), this);
		connect(acNurbs, SIGNAL(triggered()), this, SLOT(sketchNurbs()));
		//sketcherMenu->addAction(acNurbs);
		acListSketchPrimitive->addAction(acNurbs);

		mainToolBar->addSeparator();
	}

	ChildWindow& MainWindow::currentChildWindow() const
	{
		assert(currentChild);
		return *currentChild;
	}

	Model& MainWindow::currentModel() const
	{
		const ChildWindow& window = currentChildWindow();
		Model* model = window.getView().getModel();
		assert(model);
		return *model;
	}

	View& MainWindow::currentView() const
	{
		const ChildWindow& window = currentChildWindow();
		return window.getView();
	}

	void MainWindow::shapesVisChanged(bool)
	{
	}

	void MainWindow::about()
	{
		QString msg(tr("SimpleCAD %1.%2").arg(VERSION_MAJOR).arg(VERSION_MINOR));
		QMessageBox::information(this, tr("О программе"), msg);
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
			disconnect(currentChild, SIGNAL(selectionChanged()),
				this, SLOT(viewSelectionChanged()));
		}

		currentChild = qobject_cast<ChildWindow*>(window);
		shapesTreeView->setModel(currentModel().getQModel());

		connect(currentChild, SIGNAL(selectionChanged()),
			this, SLOT(viewSelectionChanged()));
		connect(currentChild, SIGNAL(closed()),
			this, SLOT(childWindowClosed()));

		enableCreateActions();
		enableViewActions();
		enableSketch();
		enableModOperations();
	}

	void MainWindow::childWindowClosed()
	{
		currentChild = 0;

		disableCreateActions();
		disableViewActions();
		disableSketch();
		disableModOperations();

		controller->cancelAllCommands();
	}

	void MainWindow::viewSelectionChanged()
	{
		View& view = currentView();
		bool enabled = view.getSelectedShape() != 0;
		acRemove->setEnabled(enabled);
	}

	void MainWindow::setDatumPlane()
	{
		const Model& model = currentModel();

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
		const Model& model = currentModel();

		//model->showDatumPlane();
	}

	void MainWindow::hideDatumPlane()
	{
		const Model& model = currentModel();

		//model->hideDatumPlane();
	}

	void MainWindow::viewFront()
	{
		ChildWindow& window = currentChildWindow();
		window.getView().viewFront();
	}

	void MainWindow::viewBack()
	{
		ChildWindow& window = currentChildWindow();
		window.getView().viewBack();
	}

	void MainWindow::viewTop()
	{
		ChildWindow& window = currentChildWindow();
		window.getView().viewTop();
	}

	void MainWindow::viewBottom()
	{
		ChildWindow& window = currentChildWindow();
		window.getView().viewBottom();
	}

	void MainWindow::viewLeft()
	{
		ChildWindow& window = currentChildWindow();
		window.getView().viewLeft();
	}

	void MainWindow::viewRight()
	{
		ChildWindow& window = currentChildWindow();
		window.getView().viewRight();
	}

	void MainWindow::viewAll()
	{
		ChildWindow& window = currentChildWindow();
		window.getView().viewAll();
	}

	void MainWindow::viewAxometric()
	{
		ChildWindow& window = currentChildWindow();
		window.getView().viewAxometric();
	}

	void MainWindow::selectNeutral()
	{
		Model& model = currentModel();
		//if (model)
			//model->selectNeutral();
	}

	void MainWindow::selectVertex()
	{
		const Model& model = currentModel();
		//if (model)
			//model->selectVertex();
	}

	void MainWindow::selectEdge()
	{
		const Model& model = currentModel();
		//if (model)
			//model->selectEdge();
	}

	void MainWindow::selectFace()
	{
		const Model& model = currentModel();
		//if (model)
			//model->selectFace();
	}

	void MainWindow::selectSolid()
	{
		const Model& model = currentModel();
		//if (model)
			//model->selectSolid();
	}

	void MainWindow::createBox()
	{
		Model& model = currentModel();
		CreatePrimitiveCommand* cmd = new CreatePrimitiveCommand(model,
			CreatePrimitiveCommand::Box);

		try
		{
			controller->execCommand(cmd);
		}
		catch(Common::Exception& ex)
		{
			QMessageBox::critical(this, tr("Ошибка"), ex.what());
		}
	}

	void MainWindow::createCylinder()
	{
		Model& model = currentModel();
		CreatePrimitiveCommand* cmd = new CreatePrimitiveCommand(model,
			CreatePrimitiveCommand::Cylinder);

		try
		{
			controller->execCommand(cmd);
		}
		catch(Common::Exception& ex)
		{
			QMessageBox::critical(this, tr("Ошибка"), ex.what());
		}
	}

	void MainWindow::createSphere()
	{
		Model& model = currentModel();
		CreatePrimitiveCommand* cmd = new CreatePrimitiveCommand(model,
			CreatePrimitiveCommand::Sphere);

		try
		{
			controller->execCommand(cmd);
		}
		catch(Common::Exception& ex)
		{
			QMessageBox::critical(this, tr("Ошибка"), ex.what());
		}
	}

	void MainWindow::createCone()
	{
		Model& model = currentModel();
		CreatePrimitiveCommand* cmd = new CreatePrimitiveCommand(model,
			CreatePrimitiveCommand::Cone);

		try
		{
			controller->execCommand(cmd);
		}
		catch(Common::Exception& ex)
		{
			QMessageBox::critical(this, tr("Ошибка"), ex.what());
		}
	}

	void MainWindow::createTorus()
	{
		Model& model = currentModel();
		CreatePrimitiveCommand* cmd = new CreatePrimitiveCommand(model,
			CreatePrimitiveCommand::Torus);

		try
		{
			controller->execCommand(cmd);
		}
		catch(Common::Exception& ex)
		{
			QMessageBox::critical(this, tr("Ошибка"), ex.what());
		}
	}

	void MainWindow::createPlane()
	{
		Model& model = currentModel();
		CreatePrimitiveCommand* cmd = new CreatePrimitiveCommand(model,
			CreatePrimitiveCommand::Plane);

		try
		{
			controller->execCommand(cmd);
		}
		catch(Common::Exception& ex)
		{
			QMessageBox::critical(this, tr("Ошибка"), ex.what());
		}
	}

	void MainWindow::createEllipsoid()
	{
		Model& model = currentModel();
		CreatePrimitiveCommand* cmd = new CreatePrimitiveCommand(model,
			CreatePrimitiveCommand::Ellipsoid);

		try
		{
			controller->execCommand(cmd);
		}
		catch(Common::Exception& ex)
		{
			QMessageBox::critical(this, tr("Ошибка"), ex.what());
		}
	}

	void MainWindow::createSketch()
	{
		View& view = currentView();
		CreateSketchCommand* cmd = new CreateSketchCommand(SbPlane(SbVec3f(0, 0, 1), 0), view);

		try
		{
			controller->execCommand(cmd);
		}
		catch(Common::Exception& ex)
		{
			QMessageBox::critical(this, tr("Ошибка"), ex.what());
		}
	}

	void MainWindow::sketchPolyline()
	{
		using namespace Sketcher;
		using namespace Common;

		View& view = currentView();

		CommandMessage* msg = new CommandMessage(Sketcher::ptPolyline);
		SendMessageCommand* cmd = new SendMessageCommand(view, msg);

		try
		{
			controller->execCommand(cmd);
		}
		catch(Common::Exception& ex)
		{
			QMessageBox::critical(this, tr("Ошибка"), ex.what());
		}
	}

	void MainWindow::sketchRectangle()
	{
		using namespace Sketcher;
		using namespace Common;

		View& view = currentView();

		CommandMessage* msg = new CommandMessage(Sketcher::ptRectangle);
		SendMessageCommand* cmd = new SendMessageCommand(view, msg);

		try
		{
			controller->execCommand(cmd);
		}
		catch(Common::Exception& ex)
		{
			QMessageBox::critical(this, tr("Ошибка"), ex.what());
		}
	}

	void MainWindow::sketchCircle()
	{
		using namespace Sketcher;
		using namespace Common;

		View& view = currentView();

		CommandMessage* msg = new CommandMessage(Sketcher::ptCircle);
		SendMessageCommand* cmd = new SendMessageCommand(view, msg);

		try
		{
			controller->execCommand(cmd);
		}
		catch(Common::Exception& ex)
		{
			QMessageBox::critical(this, tr("Ошибка"), ex.what());
		}
	}

	void MainWindow::sketchArc()
	{
		using namespace Sketcher;
		using namespace Common;

		View& view = currentView();

		CommandMessage* msg = new CommandMessage(Sketcher::ptArc);
		SendMessageCommand* cmd = new SendMessageCommand(view, msg);

		try
		{
			controller->execCommand(cmd);
		}
		catch(Common::Exception& ex)
		{
			QMessageBox::critical(this, tr("Ошибка"), ex.what());
		}
	}

	void MainWindow::sketchNurbs()
	{
		/*using namespace Sketcher;
		using namespace Common;

		View& view = currentView();

		CommandMessage* msg = new CommandMessage(CommandMessage::CreateNurbs);
		SendMessageCommand* cmd = new SendMessageCommand(view, msg);
		controller->execCommand(cmd);*/
	}

	void MainWindow::removeShape()
	{
		if (QMessageBox::question(this, tr("Удаление объекта"),
				tr("Удалить объект?"), QMessageBox::Yes
				|QMessageBox::No | QMessageBox::Cancel, QMessageBox::No)
				!= QMessageBox::Yes)
			return;

		View& view = currentView();
		const ViewerShape* shape = view.getSelectedShape();
		Model& model = currentModel();
		RemoveCommand* cmd = new RemoveCommand(model, *shape);

		try
		{
			controller->execCommand(cmd);
		}
		catch(Common::Exception& ex)
		{
			QMessageBox::critical(this, tr("Ошибка"), ex.what());
		}
	}

	void MainWindow::moveShape()
	{
		View& view = currentView();
		view.manipulateShape(acMove->isChecked());
	}

	void MainWindow::booleanOperation()
	{
		Model& model = currentModel();
		View& view = currentView();
		BooleanCommand* cmd = new BooleanCommand(model, view, BooleanCommand::Fuse);

		try
		{
			controller->execCommand(cmd);
		}
		catch(Common::Exception& ex)
		{
			QMessageBox::critical(this, tr("Ошибка"), ex.what());
		}
	}

	void MainWindow::makeFillet()
	{
		View& view = currentView();
		const ViewerShape* shape = view.getSelectedShape();
		Model& model = currentModel();
		FilletCommand* cmd = new FilletCommand(model, view, *shape);

		try
		{
			controller->execCommand(cmd);
		}
		catch(Common::Exception& ex)
		{
			QMessageBox::critical(this, tr("Ошибка"), ex.what());
		}
	}

	void MainWindow::disableCreateActions()
	{
		acList3dPrimitive->setEnabled(false);
	}

	void MainWindow::enableCreateActions()
	{
		acList3dPrimitive->setEnabled(true);
	}

	void MainWindow::disableViewActions()
	{
		acListViewType->setEnabled(false);
	}

	void MainWindow::enableViewActions()
	{
		acListViewType->setEnabled(true);
	}

	void MainWindow::disableModOperations()
	{
		acListModOperations->setEnabled(false);
	}

	void MainWindow::enableModOperations()
	{
		acListModOperations->setEnabled(true);
	}

	void MainWindow::disableSketch()
	{
		acListSketchPrimitive->setEnabled(false);
	}

	void MainWindow::enableSketch()
	{
		acListSketchPrimitive->setEnabled(true);
	}

}

