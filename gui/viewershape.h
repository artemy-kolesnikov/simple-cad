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

#include <TopoDS_Shape.hxx>
#include <QString>

#include "viewprovider.h"

class SoGroup;

namespace Gui
{

	class ViewerShape
	{
		friend class ViewProvider;
	public:
		ViewerShape(QString name, TopoDS_Shape shape);
		~ViewerShape();

		QString getName() const;
		TopoDS_Shape getShape() const;

	private:
		void computeShape();

		SoGroup* getSoGroup() const;

	private:
		TopoDS_Shape shape;
		QString name;
		SoGroup* rootGroup;
		SoGroup* faces;
		SoGroup* edges;
		SoGroup* vertices;
	};

}

#endif // VIEWER_SHAPE_HEADER

