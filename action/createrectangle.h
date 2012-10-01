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

#ifndef CREATE_RECTANGLE_ACTION_HEADER
#define CREATE_RECTANGLE_ACTION_HEADER

#include <TopoDS_Shape.hxx>
#include <gp_Pnt.hxx>

#include <QObject>

#include "create3dprimitive.h"

namespace Action
{

	class CreateRectangle : public Create3dPrimitive
	{
	public:
		CreateRectangle(const gp_Pnt& point, float width, float height);

		virtual QString getName() const
		{
			return QObject::tr("Create rectangle");
		}

	private:
		virtual TopoDS_Shape executeCreation() const;

	private:
		gp_Pnt point;
		float width;
		float height;
	};

}

#endif // CREATE_RECTANGLE_ACTION_HEADER

