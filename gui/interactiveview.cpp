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

#include "interactiveview.h"

#include <Inventor/Qt/viewers/SoQtViewer.h>
#include <Inventor/SoPath.h>
#include <Inventor/events/SoEvents.h>
#include <Inventor/nodes/SoEventCallback.h>
#include <Inventor/nodes/SoSelection.h>
#include <Inventor/nodes/SoSeparator.h>

namespace Gui
{
	InteractiveView::InteractiveView(QObject* parent) : QObject(parent)
	{
		rootSeparator = new SoSeparator();
		rootSeparator->ref();

		selectableSeparator = new SoSeparator();
		selectableSeparator->ref();

		unselectableSeparator = new SoSeparator();
		unselectableSeparator->ref();

		rootSelection = new SoSelection();
		rootSelection->ref();
		rootSelection->addSelectionCallback(&InteractiveView::selectionCallback, this);
		rootSelection->addDeselectionCallback(&InteractiveView::deselectionCallback, this);

		eventCallbacker = new SoEventCallback();
		eventCallbacker->ref();
		eventCallbacker->addEventCallback(SoEvent::getClassTypeId(),
			&InteractiveView::eventCallback, this);

		rootSelection->addChild(eventCallbacker);
		rootSelection->addChild(selectableSeparator);

		rootSeparator->addChild(unselectableSeparator);
		rootSeparator->addChild(rootSelection);
	}

	InteractiveView::~InteractiveView()
	{
		rootSelection->unref();
		rootSeparator->unref();
		selectableSeparator->unref();
		unselectableSeparator->unref();
		eventCallbacker->unref();
	}

	SoSeparator* InteractiveView::getSeparator() const
	{
		return rootSeparator;
	}

	void InteractiveView::eventCallback(void* data, SoEventCallback* callback)
	{
		InteractiveView* self = reinterpret_cast<InteractiveView*>(data);
		assert(self);

		const SoEvent* event = callback->getEvent();

		const SbVec2f point(event->getPosition());
		const SbVec3f normal;//(event->getViewDirection());
		const SoPickedPoint* pickedPoint = callback->getPickedPoint();

        if (event->getTypeId().isDerivedFrom(SoKeyboardEvent::getClassTypeId()))
		{
            const SoKeyboardEvent* keyEvent = static_cast<const SoKeyboardEvent*>(event);
			if (self->keyPressed(keyEvent->getKey()))
				callback->setHandled();
        }
        else if (event->getTypeId().isDerivedFrom(SoMouseButtonEvent::getClassTypeId()))
		{
            const SoMouseButtonEvent* mouseEvent = static_cast<const SoMouseButtonEvent*>(event);
            int button = mouseEvent->getButton();
            SbBool press = mouseEvent->getState() == SoButtonEvent::DOWN ? TRUE : FALSE;

            if (self->mouseButtonPressed(button, press, point, normal, pickedPoint))
                callback->setHandled();
        }
        else if (event->getTypeId().isDerivedFrom(SoLocation2Event::getClassTypeId()))
		{
            if (self->mouseMove(point, normal, pickedPoint))
                callback->setHandled();
        }
	}

	void InteractiveView::selectionCallback(void* data, SoPath* path)
	{
		InteractiveView* self = reinterpret_cast<InteractiveView*>(data);
		assert(self);

		self->pathSelected(path);
	}

	void InteractiveView::deselectionCallback(void* data, SoPath* path)
	{
		InteractiveView* self = reinterpret_cast<InteractiveView*>(data);
		assert(self);

		self->pathDeselected(path);
	}

	void InteractiveView::emitShapeCreated(TopoDS_Shape& shape)
	{
		Q_EMIT shapeCreated(shape);
	}

	const SoQtViewer& InteractiveView::getViewer() const
	{
		return *viewer;
	}

	void InteractiveView::setViewer(SoQtViewer* viewer)
	{
		this->viewer = viewer;
	}

}

