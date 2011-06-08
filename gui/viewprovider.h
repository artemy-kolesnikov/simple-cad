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

#include <TopoDS_Shape.hxx>
#include <map>
#include <utility>
#include <list>

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

		TopoDS_Shape getTopoShape(const ViewerShape* shape) const;
		const ViewerShape* getViewerShape(const TopoDS_Shape& shape) const;

		void display(ViewerShape* shape);
		void remove(ViewerShape* shape);

		/*typedef std::pair<const SoGroup*, const TopoDS_Shape> SoShapePair;
		typedef std::pair<TopoDS_Shape, const SoGroup*> TopoShapePair;

		typedef std::list<SoShapePair> SoShapeList;
		typedef std::list<TopoShapePair> TopoShapeList;*/

		static void computeEdges(SoGroup* edgeRoot, const TopoDS_Shape &shape);
		static void computeVertices(SoGroup* vertexRoot, const TopoDS_Shape &shape);
		static void computeFaces(SoGroup* faceRoot, const TopoDS_Shape &shape, double deflection);

	private:
		typedef std::pair<TopoDS_Shape, const ViewerShape*> TopoShapePair;
		typedef std::list<TopoShapePair> TopoShapeList;

		//TopoShapeList::const_iterator findSoShape(const TopoDS_Shape& shape);
		//void addSoShapePair(const SoShapePair& shapePair);
		//void removeSoShapePair(const SoShapePair& shapePair);

		TopoShapeList::const_iterator findViewerShape(const TopoDS_Shape& shape) const;
		TopoShapeList::iterator findViewerShape(const TopoDS_Shape& shape);
		void addTopoShapePair(const TopoShapePair& shapePair);
		void removeTopoShapePair(const TopoShapePair& shapePair);

	private:
		//std::map<SoShape*, TopoDS_Shape> soShapeMap;
		//std::map<TopoDS_Shape, const ViewerShape*> topoShapeMap;

		TopoShapeList topoShapeList;
		std::map<const ViewerShape*, TopoDS_Shape> viewerShapeMap;

		InventorViewer& viewer;
	};

}

#endif // VIEW_PROVIDER_HEADER

