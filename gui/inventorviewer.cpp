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

#include "inventorviewer.h"
#include "interactiveview.h"

#include <Inventor/nodes/SoEventCallback.h>
#include <Inventor/nodes/SoNode.h>
#include <Inventor/nodes/SoOrthographicCamera.h>
#include <Inventor/nodes/SoSelection.h>

namespace Gui
{

	InventorViewer::InventorViewer(QWidget* parent) :
		inherited(parent, "", FALSE,
			SoQtFullViewer::BUILD_NONE, SoQtViewer::EDITOR),
		interactiveView(0)
	{
		setBackgroundColor(SbColor(0.39, 0.58, 0.93));
		setCamera(new SoOrthographicCamera());

		rootSelection = new SoSelection();
		rootSelection->addSelectionCallback(&InventorViewer::selectionCallback, this);
		rootSelection->addDeselectionCallback(&InventorViewer::deselectionCallback, this);
		setSceneGraph(rootSelection);

		eventCallbacker = new SoEventCallback();
		eventCallbacker->ref();
		eventCallbacker->addEventCallback(SoEvent::getClassTypeId(),
			&InventorViewer::eventCallback, this);

		viewerEvents.reset(new ViewerEvents());
	}

	InventorViewer::~InventorViewer()
	{
		eventCallbacker->unref();
	}

	void InventorViewer::setCameraOrientation(const SbRotation& rotation)
	{
		getCamera()->orientation.setValue(rotation);
	}

	const ViewerEvents* InventorViewer::getEventObject() const
	{
		return viewerEvents.get();
	}

	void InventorViewer::setInteractiveView(InteractiveView* view)
	{
		if (interactiveView)
			rootSelection->removeChild(interactiveView->getSeparator());

		interactiveView = view;
		interactiveView->setViewer(this);
		rootSelection->addChild(interactiveView->getSeparator());
	}

	InteractiveView* InventorViewer::getInteractiveView() const
	{
		return interactiveView;
	}

	SoSelection* InventorViewer::getRootNode()
	{
		return rootSelection;
	}

	void InventorViewer::setSceneGraph(SoNode* root)
	{
		inherited::setSceneGraph(root);
	}

	SoNode* InventorViewer::getSceneGraph()
	{
		return inherited::getSceneGraph();
	}

	void InventorViewer::eventCallback(void* data, SoEventCallback* callback)
	{
		InventorViewer* self = reinterpret_cast<InventorViewer*>(data);
		assert(self);
		self->eventHandler(callback);
	}

	void InventorViewer::selectionCallback(void* data, SoPath* path)
	{
		InventorViewer* self = reinterpret_cast<InventorViewer*>(data);
		assert(self);
		self->selectionHandler(path);
	}

	void InventorViewer::deselectionCallback(void* data, SoPath* path)
	{
		InventorViewer* self = reinterpret_cast<InventorViewer*>(data);
		assert(self);
		self->deselectionHandler(path);
	}

	void InventorViewer::eventHandler(SoEventCallback* callback)
	{
	}

	void InventorViewer::selectionHandler(SoPath* path)
	{
		Q_EMIT viewerEvents->pathSelected(path);
	}

	void InventorViewer::deselectionHandler(SoPath* path)
	{
		Q_EMIT viewerEvents->pathDeselected(path);
	}

}

