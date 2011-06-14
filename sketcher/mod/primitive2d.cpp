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

#include "primitive2d.h"

#include <Inventor/Qt/viewers/SoQtViewer.h>
#include <Inventor/actions/SoHandleEventAction.h>
#include <Inventor/elements/SoViewVolumeElement.h>
#include <Inventor/events/SoEvents.h>
#include <Inventor/sensors/SoFieldSensor.h>

#include <sketcherview.h>

namespace Sketcher
{

	Primitive2D::Primitive2D(SketcherView& sketcherView) :
		sketcherView(sketcherView)
	{
		eventAction = 0;

		coordFieldSensor.reset(new SoFieldSensor(&Primitive2D::fieldSensorCallback, this));
		coordFieldSensor->setPriority(0);

		eventsHandles = false;

		events.reset(new Primitive2DEvents());
	}

	Primitive2D::~Primitive2D()
	{
	}

	void Primitive2D::handleEvents(bool handle)
	{
		eventsHandles = handle;
	}

	bool Primitive2D::isEventsHandles() const
	{
		return eventsHandles;
	}

	Primitive2DEvents* Primitive2D::getEvents() const
	{
		return events.get();
	}

	void Primitive2D::handleEvent(SoHandleEventAction* action)
	{
		if (!eventsHandles)
		{
			SoSeparator::handleEvent(action);
			return;
		}

		eventAction = action;
		const SoEvent* event = action->getEvent();

		SbViewportRegion viewRegion = sketcherView.getViewer().getViewportRegion();
		SbVec2f normalizedPos(event->getNormalizedPosition(viewRegion));

		SbViewVolume volume;
		volume = SoViewVolumeElement::get(eventAction->getState());
		SbVec3f curPos(sketcherView.projectPoint(normalizedPos, volume));

        if (event->getTypeId().isDerivedFrom(SoKeyboardEvent::getClassTypeId()))
		{
            const SoKeyboardEvent* keyEvent = static_cast<const SoKeyboardEvent*>(event);

			int key = keyEvent->getKey();

			if (key == SoKeyboardEvent::RETURN)
				emitCreated();
			else if (key == SoKeyboardEvent::ESCAPE)
				emitCreationCanceled();

			if (keyPressed(key))
				action->setHandled();
        }
        else if (event->getTypeId().isDerivedFrom(SoMouseButtonEvent::getClassTypeId()))
		{
            const SoMouseButtonEvent* mouseEvent = static_cast<const SoMouseButtonEvent*>(event);
            int button = mouseEvent->getButton();
            SbBool press = mouseEvent->getState() == SoButtonEvent::DOWN ? TRUE : FALSE;

            if (mouseButtonPressed(button, press, curPos))
                action->setHandled();
        }
        else if (event->getTypeId().isDerivedFrom(SoLocation2Event::getClassTypeId()))
		{
            if (mouseMove(curPos))
                action->setHandled();
        }
	}

	void Primitive2D::attachFieldSensor(SoField* field)
	{
		coordFieldSensor->attach(field);
	}

	void Primitive2D::fieldSensorCallback(void* userData, SoSensor* sensor)
	{
		Primitive2D* self = static_cast<Primitive2D*>(userData);
		assert(self);

		self->fieldSensorCallback(sensor);
	}

	void Primitive2D::emitCreated()
	{
		events->created(this);
	}

	void Primitive2D::emitCreationCanceled()
	{
		events->creationCanceled(this);
	}

}

