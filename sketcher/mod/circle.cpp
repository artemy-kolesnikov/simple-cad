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

#include "circle.h"

#include <Inventor/Qt/SoQtRenderArea.h>
#include <Inventor/events/SoEvents.h>
#include <Inventor/nodes/SoCoordinate3.h>
#include <Inventor/nodes/SoLineSet.h>
#include <Inventor/nodes/SoPointSet.h>
#include <Inventor/sensors/SoFieldSensor.h>

#include <cmath>

#include <QDebug>

namespace Sketcher
{

	Circle::Circle(SketcherView& sketcherView) :
		ResizePolygon(sketcherView)
	{
		radiusCoord = new SoCoordinate3();
		radiusCoord->ref();
		addChild(radiusCoord);

		radiusPoints = new SoPointSet();
		radiusPoints->ref();

		radiusLine = new SoLineSet();
		radiusLine->ref();
		addChild(radiusLine);

		coordFieldSensor.reset(new SoFieldSensor(&Circle::fieldSensorCallback, this));
		coordFieldSensor->setPriority(0);
	}

	Circle::~Circle()
	{
		radiusLine->unref();
		radiusPoints->unref();
		radiusCoord->unref();
	}

	void Circle::calculatePoints(const SbVec3f& initPos, const SbVec3f& curPos,
			SoCoordinate3& coord)
	{
		radiusCoord->point.set1Value(0, initPos);
		radiusCoord->point.set1Value(1, curPos);

		float xdist = initPos[0] - curPos[0];
		float ydist = initPos[1] - curPos[1];
		float radius = sqrt(xdist*xdist + ydist*ydist);

		const float step = 2*M_PI / 360;

		float x1, y1;
		float x2 = initPos[0] + radius;
		float y2 = initPos[1];

		int num = 0;

		for (float i = 0; i <= 2*M_PI; i += step)
		{
			x1 = x2;
			y1 = y2;
			x2 = radius * cos(i) + initPos[0];
			y2 = radius * sin(i) + initPos[1];

			coord.point.set1Value(num++, SbVec3f(x1, y1, 0));
			coord.point.set1Value(num++, SbVec3f(x2, y2, 0));
		}
	}

	void Circle::creationEnd()
	{
		radiusCoord->point.deleteValues(0);
	}

	void Circle::fieldSensorCallback(void* userData, SoSensor* sensor)
	{
		Circle* self = static_cast<Circle*>(userData);
		assert(self);

		int numPoints = self->radiusCoord->point.getNum();
		self->radiusPoints->numPoints.setValue(numPoints);
		self->radiusLine->numVertices.setValue(numPoints);
	}

}
