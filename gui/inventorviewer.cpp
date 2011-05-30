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

#include <Inventor/nodes/SoNode.h>
#include <Inventor/nodes/SoSelection.h>

#include <GL/gl.h>

namespace Gui
{

	InventorViewer::InventorViewer(QWidget* parent) :
		inherited(parent, "", FALSE,
			SoQtFullViewer::BUILD_NONE, SoQtViewer::EDITOR),
		interactiveView(0)
	{
		rootSelection = new SoSelection();
		rootSelection->ref();

		setSceneGraph(rootSelection);
	}

	InventorViewer::~InventorViewer()
	{
		rootSelection->unref();
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

}

