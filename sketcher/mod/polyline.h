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

#ifndef POLYLINE_HEADER
#define POLYLINE_HEADER

#include "primitive2d.h"

#include <Inventor/SbVec3f.h>

class SoCoordinate3;
class SoLineSet;
class SoPointSet;
class SoSensor;

namespace Sketcher
{

	class Polyline : public Primitive2D
	{
	public:
		Polyline(SketcherView& sketcherView);
		~Polyline();

	protected:
		virtual bool keyPressed(int key);
    	virtual bool mouseMove(const SbVec3f& pos);
    	virtual bool mouseButtonPressed(int Button, bool pressed, const SbVec3f &pos);

		virtual void fieldSensorCallback(SoSensor*);

	private:
		void clearPoints();
		void leftButtonPressed(const SbVec3f& pos);

	private:
		SoCoordinate3* coord;
		SoPointSet* pointSet;
		SoLineSet* lineSet;
		int currentCoord;
		bool highlight;
		SbVec3f lastPoint;
	};

}

#endif // POLYLINE_HEADER

