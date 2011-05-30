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
		ResizePolygon(sketcherView)
	{
	}

	void Rectangle::calculatePoints(const SbVec3f& initPos, const SbVec3f& curPos,
			SoCoordinate3& coord)
	{
		int deltaWidth = abs(initPos[0] - curPos[0]) * 2;
		int deltaHeight = abs(initPos[1] - curPos[1]) * 2;

		SbVec3f leftTop, rightTop, leftBottom, rightBottom;

		calculateRect(initPos, leftTop, rightTop, leftBottom, rightBottom,
			initWidth + deltaWidth, initHeight + deltaHeight);

		coord.point.set1Value(0, leftTop);
		coord.point.set1Value(1, rightTop);
		coord.point.set1Value(2, rightBottom);
		coord.point.set1Value(3, leftBottom);
		coord.point.set1Value(4, leftTop);
	}

}

