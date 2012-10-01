#include "viewprovider.h"

#include <Inventor/nodes/SoSelection.h>

#include "inventorviewer.h"
#include "viewershape.h"

#include <algorithm>

namespace std
{

	template<class _T1, class _T2>
	inline bool	operator==(const std::pair<_T1, _T2>& p, const SoPath* path)
	{
		return *p.first == *path;
	}

	template<class _T1, class _T2>
	inline bool	operator==(const std::pair<_T1, _T2>& p, const SoGroup* group)
	{
		return p.first == group;
	}

	template<class _T1, class _T2>
	inline bool	operator==(const std::pair<_T1, _T2>& p, const Gui::Shape& shape)
	{
		return p.first == shape;
	}

}

namespace Gui
{


	ViewProvider::ViewProvider(InventorViewer& viewer) : viewer(viewer)
	{
	}

	ViewProvider::~ViewProvider()
	{
	}

	bool ViewProvider::getTopoShape(const ViewerShape* viewerShape, Shape& topoShape) const
	{
		bool res = false;
		std::map<const ViewerShape*, Shape>::const_iterator it = viewerShapeMap.find(viewerShape);
		if (it != viewerShapeMap.end())
		{
			topoShape = it->second;
			assert(topoShape == it->first->getShape());
			res = true;
		}

		return res;
	}

	bool ViewProvider::getViewerShape(const Shape& topoShape, const ViewerShape*& viewerShape) const
	{
		TopoShapeList::const_iterator it = findViewerShape(topoShape);
		if (it != topoShapeList.end())
			viewerShape = it->second;

		return (viewerShape != 0);
	}

	bool ViewProvider::getViewerShape(const SoGroup* group, const ViewerShape*& viewerShape) const
	{
		PathShapeList::const_iterator it = findViewerShape(group);
		if (it != pathShapeList.end())
			viewerShape = it->second;

		return (viewerShape != 0);
	}

	void ViewProvider::display(ViewerShape* shape)
	{
		viewer.getRootNode()->addChild(shape->getSoGroup());

		if (viewerShapeMap.find(shape) == viewerShapeMap.end())
			viewerShapeMap.insert(std::pair<const ViewerShape*, Shape>(shape, shape->getShape()));

		addTopoShapePair(TopoShapePair(shape->getShape(), shape));

		addPathShapePair(PathShapePair(shape->getSoGroup(), shape));
	}

	void ViewProvider::remove(const ViewerShape* shape)
	{
		removeTopoShapePair(TopoShapePair(shape->getShape(), shape));

		viewer.getRootNode()->removeChild(shape->getSoGroup());

		std::map<const ViewerShape*, Shape>::iterator it = viewerShapeMap.find(shape);
		if (it != viewerShapeMap.end())
		{
			delete it->first;
			viewerShapeMap.erase(it);
		}
	}

	ViewProvider::TopoShapeList::const_iterator ViewProvider::findViewerShape(const Shape& shape) const
	{
		return std::find(topoShapeList.begin(), topoShapeList.end(), shape);
	}

	ViewProvider::TopoShapeList::iterator ViewProvider::findViewerShape(const Shape& shape)
	{
		return std::find(topoShapeList.begin(), topoShapeList.end(), shape);
	}

	void ViewProvider::addTopoShapePair(const TopoShapePair& shapePair)
	{
		TopoShapeList::const_iterator it = findViewerShape(shapePair.first);
		if (it == topoShapeList.end())
			topoShapeList.push_back(shapePair);
	}

	void ViewProvider::removeTopoShapePair(const TopoShapePair& shapePair)
	{
		TopoShapeList::iterator it = findViewerShape(shapePair.first);
		if (it != topoShapeList.end())
			topoShapeList.erase(it);
	}

	ViewProvider::PathShapeList::const_iterator ViewProvider::findViewerShape(const SoGroup* group) const
	{
		return std::find(pathShapeList.begin(), pathShapeList.end(), group);
	}

	ViewProvider::PathShapeList::iterator ViewProvider::findViewerShape(const SoGroup* group)
	{
		return std::find(pathShapeList.begin(), pathShapeList.end(), group);
	}

	void ViewProvider::addPathShapePair(const ViewProvider::PathShapePair& shapePair)
	{
		PathShapeList::const_iterator it = findViewerShape(shapePair.first);
		if (it == pathShapeList.end())
			pathShapeList.push_back(shapePair);
	}

	void ViewProvider::removeTopoShapePair(const ViewProvider::PathShapePair& shapePair)
	{
		PathShapeList::iterator it = findViewerShape(shapePair.first);
		if (it != pathShapeList.end())
			pathShapeList.erase(it);
	}

}

