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

#include "resizepolygon.h"

#include <Inventor/Qt/SoQtRenderArea.h>
#include <Inventor/events/SoEvents.h>
#include <Inventor/nodes/SoCoordinate3.h>
#include <Inventor/nodes/SoLineSet.h>
#include <Inventor/nodes/SoPointSet.h>

#include <cmath>

#include <QDebug>

namespace Sketcher
{

	ResizePolygon::ResizePolygon(SketcherView& sketcherView) :
		Primitive2D(sketcherView)
	{
		coord = new SoCoordinate3();
		coord->ref();
		coord->point.deleteValues(0);
		addChild(coord);

		pointSet = new SoPointSet();
		pointSet->ref();
		pointSet->numPoints.setValue(0);
		addChild(pointSet);

		lineSet = new SoLineSet();
		lineSet->ref();
		lineSet->numVertices.setValue(0);
		addChild(lineSet);

		creationStarted = false;

		attachFieldSensor(&coord->point);
	}

	ResizePolygon::~ResizePolygon()
	{
		lineSet->unref();
		pointSet->unref();
		coord->unref();
	}

	bool ResizePolygon::keyPressed(int)
	{
		return false;
	}

	bool ResizePolygon::mouseMove(const SbVec3f& pos)
	{
		if (creationStarted)
			calculatePoints(initPoint, pos, *coord);

		return false;
	}

	bool ResizePolygon::mouseButtonPressed(int Button, bool pressed, const SbVec3f &pos)
	{
		switch(Button)
		{
			case SoMouseButtonEvent::BUTTON1:
			{
				if (pressed)
				{
					if (!creationStarted)
						startCreation(pos);
					else
						endCreation();
				}
				break;
			}
		}
		return true;
	}

	void ResizePolygon::fieldSensorCallback(SoSensor*)
	{
		int numPoints = coord->point.getNum();
		pointSet->numPoints.setValue(numPoints);
		lineSet->numVertices.setValue(numPoints);
	}

	void ResizePolygon::startCreation(const SbVec3f& pos)
	{
		creationStart();

		creationStarted = true;

		initPoint = pos;

		//calculatePoints(initPoint, pos, *coord);
	}

	void ResizePolygon::endCreation()
	{
		creationEnd();

		creationStarted = false;
		emitCreated();
	}

}

