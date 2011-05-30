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

#include "polyline.h"

#include <Inventor/Qt/SoQtRenderArea.h>
#include <Inventor/events/SoEvents.h>
#include <Inventor/nodes/SoCoordinate3.h>
#include <Inventor/nodes/SoLineSet.h>
#include <Inventor/nodes/SoPointSet.h>

#include <sketcherview.h>

#include <QDebug>

namespace Sketcher
{

	Polyline::Polyline(SketcherView& sketcherView) :
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

		attachFieldSensor(&coord->point);

		currentCoord = -1;

		highlight = false;
	}

	Polyline::~Polyline()
	{
		lineSet->unref();
		pointSet->unref();
		coord->unref();
	}

	bool Polyline::keyPressed(int key)
	{
		if (key == SoKeyboardEvent::RETURN)
		{
			highlight = false;
			coord->point.deleteValues(currentCoord--);
		}
		else if (key == SoKeyboardEvent::ESCAPE)
		{
			highlight = false;
			clearPoints();
		}

		return true;
	}

    bool Polyline::mouseMove(const SbVec3f& curPos)
	{
		if (currentCoord != -1 && highlight)
		{
			//SbVec3f curPos;
			//sketcherViewProvider.projectMouse(curPos);

			SbVec3f firstPoint = coord->point[0];
			SbVec3f v(curPos - firstPoint);

			const int MIN_DISTANCE = 0;//1000;

			if (v.dot(v) < MIN_DISTANCE)
				lastPoint = firstPoint;
			else
				lastPoint = curPos;

			//qDebug() << curPos[0] << curPos[1] << curPos[2];

			coord->point.set1Value(currentCoord, lastPoint);
		}

		return true;
	}

    bool Polyline::mouseButtonPressed(int Button, bool pressed, const SbVec3f& pos)
	{
		switch(Button)
		{
			case SoMouseButtonEvent::BUTTON1:
				leftButtonPressed(pos);
				break;
		}
		return true;
	}

	void Polyline::fieldSensorCallback(SoSensor*)
	{
		int numPoints = coord->point.getNum();

		pointSet->numPoints.setValue(numPoints);
		lineSet->numVertices.setValue(numPoints);
	}

	void Polyline::clearPoints()
	{
		coord->point.deleteValues(0);
		currentCoord = -1;
	}

	void Polyline::leftButtonPressed(const SbVec3f& pos)
	{
		++currentCoord;
		highlight = true;

		if (0 == currentCoord)
			lastPoint = pos;
			//sketcherViewProvider.projectPoint(lastPoint);

		coord->point.set1Value(currentCoord, lastPoint);
	}
}

