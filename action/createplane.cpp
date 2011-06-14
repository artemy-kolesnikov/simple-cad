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

#include "createplane.h"

#include <exception.h>

#include <BRepBuilderAPI_MakeFace.hxx>
#include <StdFail_NotDone.hxx>
#include <gp_Ax2.hxx>
#include <Handle_Geom_Plane.hxx>
#include <Geom_Plane.hxx>

namespace Action
{

	CreatePlane::CreatePlane(const gp_Ax3& axis, float height, float width) :
		axis(axis), height(height), width(width)
	{
	}

	TopoDS_Shape CreatePlane::executeCreation() const
	{
		try
		{
			Handle(Geom_Plane) plane = new Geom_Plane(axis);

			BRepBuilderAPI_MakeFace makeFace(plane, 0.0, height, 0.0, width);
			return makeFace.Shape();
		}
		catch(const StdFail_NotDone& ex)
		{
			throw Common::Exception(QObject::tr("Create plane error"));
		}
	}

}

