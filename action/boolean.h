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

#ifndef BOOLEAN_ACTION_HEADER
#define BOOLEAN_ACTION_HEADER

#include <combineaction.h>

#include <TopTools_HSequenceOfShape.hxx>
#include <TopoDS_Shape.hxx>

namespace Action
{

	class Boolean : public CombineAction
	{
	public:
		Boolean(const TopoDS_Shape& shape1, const TopoDS_Shape& shape2);

		virtual void execute();
		virtual TopoDS_Shape getShape() const;
		virtual std::list<TopoDS_Shape> getRemovedShapes() const;

	protected:
		virtual TopoDS_Shape execute(const TopoDS_Shape& shape1, const TopoDS_Shape& shape2) const = 0;

	private:
		TopoDS_Shape shape1;
		TopoDS_Shape shape2;
		TopoDS_Shape resShape;
	};

}

#endif // BOOLEAN_ACTION_HEADER

