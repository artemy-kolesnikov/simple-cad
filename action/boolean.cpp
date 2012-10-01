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

#include "boolean.h"

namespace Action
{

	Boolean::Boolean(const TopoDS_Shape& shape1, const TopoDS_Shape& shape2) :
		shape1(shape1), shape2(shape2)
	{
	}

	void Boolean::execute()
	{
		resShape = execute(shape1, shape2);
	}

	TopoDS_Shape Boolean::getShape() const
	{
		return resShape;
	}

	std::list<TopoDS_Shape> Boolean::getRemovedShapes() const
	{
		std::list<TopoDS_Shape> shapes;
		shapes.push_back(shape1);
		shapes.push_back(shape2);
		return shapes;
	}

}

