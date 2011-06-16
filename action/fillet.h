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

#ifndef FILLET_ACTION_HEADER
#define FILLET_ACTION_HEADER

#include "combineaction.h"
#include <TopoDS_Shape.hxx>

#include <QObject>

namespace Action
{

	class Fillet : public CombineAction
	{
	public:
		Fillet(TopoDS_Shape shape, TopoDS_Shape edge, float radius);

		virtual TopoDS_Shape getShape() const;
		virtual void execute();

		virtual std::list<TopoDS_Shape> getRemovedShapes() const;

		virtual QString getName() const
		{
			return QObject::tr("Fillet");
		}

	private:
		TopoDS_Shape resShape;
		TopoDS_Shape shape;
		TopoDS_Shape edge;
		float radius;
	};

}

#endif // FILLET_ACTION_HEADER

