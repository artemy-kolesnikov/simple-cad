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

#include "view.h"
#include "model.h"
#include "cadapplication.h"
#include "inventorviewer.h"
#include "interactiveview.h"
#include "viewershape.h"

#include <QColormap>
#include <QWheelEvent>
#include <QDebug>
#include <QList>
#include <QHBoxLayout>
#include <QFocusEvent>

#include <algorithm>

#include <BRep_Tool.hxx>
#include <Geom_Axis2Placement.hxx>
#include <Geom_Plane.hxx>
#include <Geom_Surface.hxx>
#include <TopExp_Explorer.hxx> 
#include <TopoDS.hxx>
#include <TopoDS_Face.hxx> 
#include <TopoDS_Shape.hxx>
#include <gp_Ax3.hxx>

#include <Inventor/actions/SoSearchAction.h>
#include <Inventor/nodes/SoGroup.h>
#include <Inventor/nodes/SoSelection.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoShape.h>
#include <Inventor/nodes/SoCamera.h>
#include <Inventor/nodes/SoLight.h>

#include <commandmessage.h>
#include <shape.h>
#include "viewprovider.h"

namespace Gui
{

	View::View(QWidget* parent) : QWidget(parent),
		model(0), curAction(caNone),
		modKey(mkNone), selectedShape(0)
	{
		createUI();
	}

	View::~View()
	{
	}

	void View::createUI()
	{
		inventorViewer.reset(new InventorViewer(this));

		connect(inventorViewer->getEventObject(), SIGNAL(pathSelected(SoPath*)),
			this, SLOT(pathSelected(SoPath*)));
		connect(inventorViewer->getEventObject(), SIGNAL(pathDeselected(SoPath*)),
			this, SLOT(pathDeselected(SoPath*)));

		QHBoxLayout *layout = new QHBoxLayout(this);
		layout->setContentsMargins(QMargins(0, 0, 0, 0));;
		layout->addWidget(inventorViewer->getWidget());

		//setMouseTracking(true);

		viewProvider.reset(new ViewProvider(*inventorViewer.get()));
	}

	void View::setModel(Model* model)
	{
		this->model = model;

		connect(model, SIGNAL(shapeAdded(const Shape&)),
			this, SLOT(shapeAdded(const Shape&)));
		connect(model, SIGNAL(shapeRemoved(const Shape&)),
			this, SLOT(shapeRemoved(const Shape&)));
	}

	Model* View::getModel() const
	{
		return model;
	}

	void View::setInteractiveView(InteractiveView* view)
	{
		interactiveView.reset(view);
		inventorViewer->setInteractiveView(interactiveView.get());
	}

	void View::receive(Common::Message* msg)
	{
		Sketcher::CommandMessage* message = dynamic_cast<Sketcher::CommandMessage*>(msg);
		if (message && interactiveView.get())
			interactiveView->receive(message);
	}

	const ViewerShape* View::getSelectedShape() const
	{
		return selectedShape;
	}

	void View::viewFront()
	{
		float root = (float)(sqrt(2.0)/2.0);
		inventorViewer->setCameraOrientation(SbRotation(-root, 0, 0, -root));
		inventorViewer->viewAll();
	}

	void View::viewBack()
	{
		float root = (float)(sqrt(2.0)/2.0);
		inventorViewer->setCameraOrientation(SbRotation(0, root, root, 0));
		inventorViewer->viewAll();
	}

	void View::viewTop()
	{
		inventorViewer->setCameraOrientation(SbRotation(0, 0, 0, 1));
		inventorViewer->viewAll();
	}

	void View::viewBottom()
	{
		inventorViewer->setCameraOrientation(SbRotation(-1, 0, 0, 0));
		inventorViewer->viewAll();
	}

	void View::viewLeft()
	{
		inventorViewer->setCameraOrientation(SbRotation(-0.5, 0, 0, -0.5));
		inventorViewer->viewAll();
	}

	void View::viewRight()
	{
		inventorViewer->setCameraOrientation(SbRotation(0.5, 0, 0, 0.5));
		inventorViewer->viewAll();
	}

	void View::viewAll()
	{
		inventorViewer->viewAll();
	}

	void View::viewAxometric()
	{
        inventorViewer->setCameraOrientation(SbRotation
            (-0.353553f, -0.146447f, -0.353553f, -0.853553f));
		inventorViewer->viewAll();
	}

	void View::viewDatumPlane()
	{
		//view->SetFront();
	}

	void View::shapeAdded(const Shape& shape)
	{
		viewProvider->display(new ViewerShape("", shape));
		inventorViewer->viewAll();
	}

	void View::shapeRemoved(const Shape& shape)
	{
		const ViewerShape* viewerShape = 0;
		viewProvider->getViewerShape(shape, viewerShape);

		assert(viewerShape);

		viewProvider->remove(viewerShape);

		selectedShape = 0;

		Q_EMIT selectionChanged();
	}

	void View::pathSelected(SoPath* path)
	{
		int pathLength = path->getLength();
		assert(pathLength > 2);

		SoNode* node = path->getNodeFromTail(pathLength - 2);

        if (SoGroup::getClassTypeId() == node->getTypeId())
		{
			viewProvider->getViewerShape(static_cast<SoGroup*>(node), selectedShape);

			assert(selectedShape);

			selectedShape->setCentralBallManip();

			Q_EMIT selectionChanged();
		}
	}

	void View::pathDeselected(SoPath* path)
	{
		if (!selectedShape)
			return;

		selectedShape->removeManip();

		selectedShape = 0;
		Q_EMIT selectionChanged();
	}

}

