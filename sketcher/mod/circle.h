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

#ifndef CIRCLE_HEADER
#define CIRCLE_HEADER

#include "resizepolygon.h"

#include <Inventor/SbVec3f.h>

#include <memory>

class SoCoordinate3;
class SoLineSet;
class SoPointSet;
class SoSensor;
class SoFieldSensor;

namespace Sketcher
{

	class Circle : public ResizePolygon
	{
	public:
		Circle(SketcherView& sketcherView);
		~Circle();

	protected:
		virtual void calculatePoints(const SbVec3f& initPos, const SbVec3f& curPos,
			SoCoordinate3& coord);

		virtual void creationEnd();

	private:
		static void fieldSensorCallback(void* userData, SoSensor* sensor);

	private:
		SoLineSet* radiusLine;
		SoPointSet* radiusPoints;
		SoCoordinate3* radiusCoord;
		std::auto_ptr<SoFieldSensor> coordFieldSensor;
	};

}

#endif // CIRCLE_HEADER
