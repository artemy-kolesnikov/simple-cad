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

#include "rectangle.h"

#include <Inventor/Qt/SoQtRenderArea.h>
#include <Inventor/events/SoEvents.h>
#include <Inventor/nodes/SoCoordinate3.h>
#include <Inventor/nodes/SoLineSet.h>
#include <Inventor/nodes/SoPointSet.h>

#include <cmath>

#include <QDebug>

namespace
{
	const int initWidth = 0;
	const int initHeight = 0;

	void calculateRect(const SbVec3f& initPoint, SbVec3f& leftTop,
		SbVec3f& rightTop, SbVec3f& leftBottom, SbVec3f& rightBottom,
		float initWidth, float initHeight)
	{
		leftTop = rightTop = leftBottom = rightBottom = initPoint;
		leftTop[0] = initPoint[0] - initWidth / 2.0;
		leftTop[1] = initPoint[1] + initHeight / 2.0;

		rightTop[0] = initPoint[0] + initWidth / 2.0;
		rightTop[1] = initPoint[1] + initHeight / 2.0;

		rightBottom[0] = initPoint[0] + initWidth / 2.0;
		rightBottom[1] = initPoint[1] - initHeight / 2.0;

		leftBottom[0] = initPoint[0] - initWidth / 2.0;
		leftBottom[1] = initPoint[1] - initHeight / 2.0;
	}
}

namespace Sketcher
{

	Rectangle::Rectangle(SketcherView& sketcherView) :
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

	Rectangle::~Rectangle()
	{
		lineSet->unref();
		pointSet->unref();
		coord->unref();
	}

	bool Rectangle::keyPressed(int)
	{
		return false;
	}

	bool Rectangle::mouseMove(const SbVec3f& pos)
	{
		if (creationStarted)
		{
			int deltaWidth = abs(initPoint[0] - pos[0]) * 2;
			int deltaHeight = abs(initPoint[1] - pos[1]) * 2;

			SbVec3f leftTop, rightTop, leftBottom, rightBottom;

			calculateRect(initPoint, leftTop, rightTop, leftBottom, rightBottom,
				initWidth + deltaWidth, initHeight + deltaHeight);

			coord->point.set1Value(0, leftTop);
			coord->point.set1Value(1, rightTop);
			coord->point.set1Value(2, rightBottom);
			coord->point.set1Value(3, leftBottom);
			coord->point.set1Value(4, leftTop);
		}

		return false;
	}

	bool Rectangle::mouseButtonPressed(int Button, bool pressed, const SbVec3f &pos)
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

	void Rectangle::fieldSensorCallback(SoSensor*)
	{
		int numPoints = coord->point.getNum();
		pointSet->numPoints.setValue(numPoints);
		lineSet->numVertices.setValue(numPoints);
	}

	void Rectangle::startCreation(const SbVec3f& pos)
	{
		creationStarted = true;

		initPoint = pos;

		SbVec3f leftTop, rightTop, leftBottom, rightBottom;

		calculateRect(initPoint, leftTop, rightTop, leftBottom, rightBottom,
			initWidth, initHeight);

		coord->point.set1Value(0, leftTop);
		coord->point.set1Value(1, rightTop);
		coord->point.set1Value(2, rightBottom);
		coord->point.set1Value(3, leftBottom);
		coord->point.set1Value(4, leftTop);
	}

	void Rectangle::endCreation()
	{
		creationStarted = false;
		emitCreated();
	}

}

