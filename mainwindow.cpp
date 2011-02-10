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
#include <QVBoxLayout>

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
#include "positionsettingswidget.h"
#include "widgetdialog.h"
#include "primitivesettingswidget.h"

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

	QWidget* shapesWidget = new QWidget(shapesDock);
	QVBoxLayout* shapesLayout = new QVBoxLayout(shapesWidget);
	shapesDock->setWidget(shapesWidget);

	shapesTreeView = new QTreeView(shapesDock);
	shapesLayout->addWidget(shapesTreeView);

	propertiesWidget = new PropertiesWidget(this);
	shapesLayout->addWidget(propertiesWidget);

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

	QMenu* planeMenu = new QMenu(mainMenuBar);
	planeMenu->setTitle(tr("Плоскость"));
	actionMenu->addMenu(planeMenu);

	acSetDatumPlane = new QAction(tr("Установить плоскость"), this);
	planeMenu->addAction(acSetDatumPlane);
	connect(acSetDatumPlane, SIGNAL(triggered()), this, SLOT(setDatumPlane()));

	acShowDatumPlane = new QAction(tr("Показать плоскость"), this);
	planeMenu->addAction(acShowDatumPlane);
	connect(acShowDatumPlane, SIGNAL(triggered()), this, SLOT(showDatumPlane()));

	acHideDatumPlane = new QAction(tr("Спрятать плоскость"), this);
	planeMenu->addAction(acHideDatumPlane);
	connect(acHideDatumPlane, SIGNAL(triggered()), this, SLOT(hideDatumPlane()));

	actionMenu->addSeparator();

	QMenu* selectionMenu = new QMenu(mainMenuBar);
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

	QMenu* menuCreate = new QMenu(mainMenuBar);
	menuCreate->setTitle(tr("Создать"));
	actionMenu->addMenu(menuCreate);

	acBox = new QAction(tr("Брусок"), this);
	connect(acBox, SIGNAL(triggered()), this, SLOT(createBox()));
	menuCreate->addAction(acBox);

	acCylinder = new QAction(tr("Цилиндр"), this);
	connect(acCylinder, SIGNAL(triggered()), this, SLOT(createCylinder()));
	menuCreate->addAction(acCylinder);

	acSphere = new QAction(tr("Сфера"), this);
	connect(acSphere, SIGNAL(triggered()), this, SLOT(createSphere()));
	menuCreate->addAction(acSphere);

	acCone = new QAction(tr("Конус"), this);
	connect(acCone, SIGNAL(triggered()), this, SLOT(createCone()));
	menuCreate->addAction(acCone);

	acTorus = new QAction(tr("Тор"), this);
	connect(acTorus, SIGNAL(triggered()), this, SLOT(createTorus()));
	menuCreate->addAction(acTorus);

	acPlane = new QAction(tr("Плоскость"), this);
	connect(acPlane, SIGNAL(triggered()), this, SLOT(createPlane()));
	menuCreate->addAction(acPlane);

	acEllipsoid = new QAction(tr("Еллипсоид"), this);
	connect(acEllipsoid, SIGNAL(triggered()), this, SLOT(createEllipsoid()));
	menuCreate->addAction(acEllipsoid);

	QMenu* menuOperations = new QMenu(mainMenuBar);
	menuOperations->setTitle(tr("Операции"));
	actionMenu->addMenu(menuOperations);

	/*acStamp = new QAction(tr("Выдавливание"), this);
	connect(acStamp, SIGNAL(triggered()), this, SLOT(makePrism()));
	menuOperations->addAction(acStamp);

	acRotation = new QAction(tr("Вращение"), this);
	menuOperations->addAction(acRotation);

	acAddition = new QAction(tr("Сложение"), this);
	menuOperations->addAction(acAddition);

	acSubstract = new QAction(tr("Вычитание"), this);
	menuOperations->addAction(acSubstract);*/

	acFusion = new QAction(tr("Сложение"), this);
	connect(acFusion, SIGNAL(triggered()), this, SLOT(shapesFusion()));
	menuOperations->addAction(acFusion);

	acCommon = new QAction(tr("Пересечение"), this);
	connect(acCommon, SIGNAL(triggered()), this, SLOT(shapesCommon()));
	menuOperations->addAction(acCommon);

	acCut = new QAction(tr("Вычитание"), this);
	connect(acCut, SIGNAL(triggered()), this, SLOT(shapesCut()));
	menuOperations->addAction(acCut);

	acRemoveShape = new QAction(tr("Удалить"), this);
	connect(acRemoveShape, SIGNAL(triggered()), this, SLOT(removeShape()));
	actionMenu->addAction(acRemoveShape);

	mainToolBar = new QToolBar(this);
	addToolBar(mainToolBar);
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

	boost::shared_ptr<AIS_SequenceOfInteractive> shapes = model->getCurrentShapes();

	Handle(AIS_Shape) shape;

	if (shapes->Length() == 1)
		shape = Handle(AIS_Shape)::DownCast(shapes->Value(1));

	propertiesWidget->setShape(shape);
}

void MainWindow::makePrism()
{
	Model* model = currentModel();
	if (!model)
		return;

	boost::shared_ptr<AIS_SequenceOfInteractive> shapes = model->getCurrentShapes();
	if (shapes->Length() == 1)
	{
		Handle(AIS_Shape) shape = Handle(AIS_Shape)::DownCast(shapes->Value(1));
		model->makePrism(shape, 150);
	}
}

void MainWindow::setDatumPlane()
{
	Model* model = currentModel();
	if (!model)
		return;

	PositionSettingsWidget* planeWidget = new PositionSettingsWidget(this);

	gp_Pnt pt;
	gp_Dir dir;
	model->getDatumPlane(pt, dir);
	planeWidget->setPoint(pt);	
	planeWidget->setDirection(dir);

	WidgetDialog* dlg = new WidgetDialog(this);
	dlg->setCentralWidget(planeWidget);
	if (dlg->exec() == QDialog::Accepted)
	{
		gp_Pnt pt = planeWidget->getPoint();
		gp_Dir dir = planeWidget->getDirection();

		model->setDatumPlane(pt, dir);
		model->showDatumPlane();
	}
}

void MainWindow::showDatumPlane()
{	
	Model* model = currentModel();
	if (!model)
		return;

	model->showDatumPlane();
}

void MainWindow::hideDatumPlane()
{
	Model* model = currentModel();
	if (!model)
		return;

	model->hideDatumPlane();
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

void MainWindow::createBox()
{
	Model* model = currentModel();
	if (!model)
		return;

	PrimitiveSettingsWidget* settingsWidget = new PrimitiveSettingsWidget(this);
	settingsWidget->setType(PrimitiveSettingsWidget::ptBox);
	WidgetDialog* dlg = new WidgetDialog(this);
	dlg->setCentralWidget(settingsWidget);
	if (dlg->exec() == QDialog::Accepted)
	{
		gp_Ax3 axis(settingsWidget->getPoint(), settingsWidget->getDir());
		float height = settingsWidget->getHeight();
		float width = settingsWidget->getWidth();
		float lenght = settingsWidget->getLength();
		model->createBox(axis, height, width, lenght);
	}
}

void MainWindow::createCylinder()
{
	Model* model = currentModel();
	if (!model)
		return;

	PrimitiveSettingsWidget* settingsWidget = new PrimitiveSettingsWidget(this);
	settingsWidget->setType(PrimitiveSettingsWidget::ptCylinder);
	WidgetDialog* dlg = new WidgetDialog(this);
	dlg->setCentralWidget(settingsWidget);
	if (dlg->exec() == QDialog::Accepted)
	{
		gp_Ax3 axis(settingsWidget->getPoint(), settingsWidget->getDir());
		float radius = settingsWidget->getRadius1();
		float height = settingsWidget->getHeight();
		float angle = settingsWidget->getAngle();
		model->createCylinder(axis, radius, height, angle);
	}
}

void MainWindow::createSphere()
{
	Model* model = currentModel();
	if (!model)
		return;

	PrimitiveSettingsWidget* settingsWidget = new PrimitiveSettingsWidget(this);
	settingsWidget->setType(PrimitiveSettingsWidget::ptSphere);
	WidgetDialog* dlg = new WidgetDialog(this);
	dlg->setCentralWidget(settingsWidget);
	if (dlg->exec() == QDialog::Accepted)
	{
		gp_Ax3 axis(settingsWidget->getPoint(), settingsWidget->getDir());
		float radius = settingsWidget->getRadius1();
		float angle = settingsWidget->getAngle();
		model->createSphere(axis, radius, angle);
	}
}

void MainWindow::createCone()
{
	Model* model = currentModel();
	if (!model)
		return;

	PrimitiveSettingsWidget* settingsWidget = new PrimitiveSettingsWidget(this);
	settingsWidget->setType(PrimitiveSettingsWidget::ptCone);
	WidgetDialog* dlg = new WidgetDialog(this);
	dlg->setCentralWidget(settingsWidget);
	if (dlg->exec() == QDialog::Accepted)
	{
		gp_Ax3 axis(settingsWidget->getPoint(), settingsWidget->getDir());
		float radius1 = settingsWidget->getRadius1();
		float radius2 = settingsWidget->getRadius2();
		float height = settingsWidget->getHeight();
		float angle = settingsWidget->getAngle();
		model->createCone(axis, radius1, radius2, height, angle);
	}
}

void MainWindow::createTorus()
{
	Model* model = currentModel();
	if (!model)
		return;

	PrimitiveSettingsWidget* settingsWidget = new PrimitiveSettingsWidget(this);
	settingsWidget->setType(PrimitiveSettingsWidget::ptTorus);
	WidgetDialog* dlg = new WidgetDialog(this);
	dlg->setCentralWidget(settingsWidget);
	if (dlg->exec() == QDialog::Accepted)
	{
		gp_Ax3 axis(settingsWidget->getPoint(), settingsWidget->getDir());
		float radius1 = settingsWidget->getRadius1();
		float radius2 = settingsWidget->getRadius2();
		float angle = settingsWidget->getAngle();
		model->createTorus(axis, radius1, radius2, angle, 0, 0);
	}
}

void MainWindow::createPlane()
{
	Model* model = currentModel();
	if (!model)
		return;

	PrimitiveSettingsWidget* settingsWidget = new PrimitiveSettingsWidget(this);
	settingsWidget->setType(PrimitiveSettingsWidget::ptPlane);
	WidgetDialog* dlg = new WidgetDialog(this);
	dlg->setCentralWidget(settingsWidget);
	if (dlg->exec() == QDialog::Accepted)
	{
		gp_Ax3 axis(settingsWidget->getPoint(), settingsWidget->getDir());
		float height = settingsWidget->getHeight();
		float width = settingsWidget->getWidth();
		model->createPlane(axis, height, width);
	}
}

void MainWindow::removeShape()
{
	Model* model = currentModel();
	if (!model)
		return;

	QString msgText("Вы действительно хотите удалить %1");

	boost::shared_ptr<AIS_SequenceOfInteractive> shapes = model->getCurrentShapes();
	if (shapes->Length() == 1)
		msgText = msgText.arg(tr("объект"));
	else
		msgText = msgText.arg(tr("выбранные объекты"));

	if (QMessageBox::question(this, tr("Удаление объекта"),
		msgText,
		QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel) == QMessageBox::Yes)
	{
		for (int i = 1; i <= shapes->Length(); ++i)
		{
			model->removeShape(shapes->Value(i));
		}
	}
}

void MainWindow::shapesFusion()
{
	Model* model = currentModel();
	if (!model)
		return;

	boost::shared_ptr<AIS_SequenceOfInteractive> shapes = model->getCurrentShapes();
	if (shapes->Length() == 2)
	{
		Handle(AIS_Shape) shape1 = Handle(AIS_Shape)::DownCast(shapes->Value(1));
		Handle(AIS_Shape) shape2 = Handle(AIS_Shape)::DownCast(shapes->Value(2));

		model->fuse(shape1, shape2);
	}
	else
		QMessageBox::critical(this, tr("Ошибка"), tr("Выберите два объекта"));
}

void MainWindow::shapesCommon()
{
	Model* model = currentModel();
	if (!model)
		return;

	boost::shared_ptr<AIS_SequenceOfInteractive> shapes = model->getCurrentShapes();
	if (shapes->Length() == 2)
	{
		Handle(AIS_Shape) shape1 = Handle(AIS_Shape)::DownCast(shapes->Value(1));
		Handle(AIS_Shape) shape2 = Handle(AIS_Shape)::DownCast(shapes->Value(2));

		model->common(shape1, shape2);
	}
	else
		QMessageBox::critical(this, tr("Ошибка"), tr("Выберите два объекта"));
}

void MainWindow::shapesCut()
{
	Model* model = currentModel();
	if (!model)
		return;

	boost::shared_ptr<AIS_SequenceOfInteractive> shapes = model->getCurrentShapes();
	if (shapes->Length() == 2)
	{
		Handle(AIS_Shape) shape1 = Handle(AIS_Shape)::DownCast(shapes->Value(1));
		Handle(AIS_Shape) shape2 = Handle(AIS_Shape)::DownCast(shapes->Value(2));

		model->cut(shape1, shape2);
	}
	else
		QMessageBox::critical(this, tr("Ошибка"), tr("Выберите два объекта"));
}

