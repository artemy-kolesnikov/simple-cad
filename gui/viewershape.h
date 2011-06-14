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

#ifndef VIEWER_SHAPE_HEADER
#define VIEWER_SHAPE_HEADER

#include <QString>
#include <memory>
#include <map>
#include <TopoDS_Shape.hxx>

#include "viewprovider.h"

#include <shape.h>

class SoGroup;
class SoTransform;
class SoCenterballManip;
class SoSensor;
class SoSelection;
class SoSeparator;
class SoVertexShape;
class SoPickedPoint;

namespace Gui
{

	class ViewerShape
	{
		friend class ViewProvider;
	public:
		ViewerShape(QString name, Shape shape);
		~ViewerShape();

		QString getName() const;
		Shape getShape() const;

		void setCentralBallManip() const;
		void removeManip() const;

		TopoDS_Shape getShape(const SoPickedPoint* point) const;

	private:
		void computeShape();
		void computeEdges(SoGroup* edgeRoot, const TopoDS_Shape &shape);
		void computeVertices(SoGroup* vertexRoot, const TopoDS_Shape &shape);
		void computeFaces(SoGroup* faceRoot, const TopoDS_Shape &shape, double deflection);

		SoGroup* getSoGroup() const;

		static void transformCallback(void* userData, SoSensor* sensor);

	private:
		Shape shape;
		QString name;
		SoGroup* rootGroup;
		SoGroup* faces;
		SoGroup* edges;
		SoGroup* vertices;
		SoTransform* transform;
		SoSeparator* rootSeparator;

		std::map<SoVertexShape*, TopoDS_Shape> shapeMap;
	};

}

#endif // VIEWER_SHAPE_HEADER

