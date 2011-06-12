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

#ifndef VIEW_PROVIDER_HEADER
#define VIEW_PROVIDER_HEADER

#include <map>
#include <utility>
#include <list>

#include <shape.h>

#include <QDebug>

class SoGroup;
class SoPath;
class SoShape;

namespace Gui
{

	class InventorViewer;
	class ViewerShape;

	class ViewProvider
	{
	public:
		ViewProvider(InventorViewer& viewer);
		~ViewProvider();

		bool getTopoShape(const ViewerShape* viewerShape, Shape& topoShape) const;
		bool getViewerShape(const Shape& topoShape, const ViewerShape*& viewerShape) const;
		bool getViewerShape(const SoGroup* group, const ViewerShape*& viewerShape) const;

		void display(ViewerShape* shape);
		void remove(const ViewerShape* shape);

		/*typedef std::pair<const SoGroup*, const Shape> SoShapePair;
		typedef std::pair<Shape, const SoGroup*> TopoShapePair;

		typedef std::list<SoShapePair> SoShapeList;
		typedef std::list<TopoShapePair> TopoShapeList;*/

		static void computeEdges(SoGroup* edgeRoot, const TopoDS_Shape &shape);
		static void computeVertices(SoGroup* vertexRoot, const TopoDS_Shape &shape);
		static void computeFaces(SoGroup* faceRoot, const TopoDS_Shape &shape, double deflection);

	private:
		/*template<class T1, class T2>
		class Pair
		{
		public:
			Pair() : first(), second() {}
			Pair(const T1& t1, const T2& t2) : first(t1), second(t2) {}

			T1 first;
			T2 second;
		};*/

		typedef std::pair<Shape, const ViewerShape*> TopoShapePair;
		typedef std::list<TopoShapePair> TopoShapeList;

		typedef std::pair<const SoGroup*, const ViewerShape*> PathShapePair;
		typedef std::list<PathShapePair> PathShapeList;

		//TopoShapeList::const_iterator findSoShape(const Shape& shape);
		//void addSoShapePair(const SoShapePair& shapePair);
		//void removeSoShapePair(const SoShapePair& shapePair);

		TopoShapeList::const_iterator findViewerShape(const Shape& shape) const;
		TopoShapeList::iterator findViewerShape(const Shape& shape);
		void addTopoShapePair(const TopoShapePair& shapePair);
		void removeTopoShapePair(const TopoShapePair& shapePair);

		PathShapeList::const_iterator findViewerShape(const SoGroup* group) const;
		PathShapeList::iterator findViewerShape(const SoGroup* group);
		void addPathShapePair(const PathShapePair& shapePair);
		void removeTopoShapePair(const PathShapePair& shapePair);

	private:
		//std::map<SoShape*, Shape> soShapeMap;
		//std::map<Shape, const ViewerShape*> topoShapeMap;

		TopoShapeList topoShapeList;
		std::map<const ViewerShape*, Shape> viewerShapeMap;

		PathShapeList pathShapeList;

		InventorViewer& viewer;
	};

}

#endif // VIEW_PROVIDER_HEADER

