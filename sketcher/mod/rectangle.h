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

#ifndef RECTANGLE_HEADER
#define RECTANGLE_HEADER

#include "resizepolygon.h"

#include <Inventor/SbVec3f.h>

class SoCoordinate3;
class SoLineSet;
class SoPointSet;
class SoSensor;

namespace Sketcher
{

	class Rectangle : public ResizePolygon
	{
	public:
		Rectangle(SketcherView& sketcherView);

	protected:
		virtual void calculatePoints(const SbVec3f& initPos, const SbVec3f& curPos,
			SoCoordinate3& coord);
	};

}

#endif // RECTANGLE_HEADER

