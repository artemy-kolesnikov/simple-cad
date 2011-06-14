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

#include "createtorus.h"

#include <exception.h>

#include <BRepPrimAPI_MakeTorus.hxx>
#include <StdFail_NotDone.hxx>
#include <gp_Ax2.hxx>

namespace Action
{

	CreateTorus::CreateTorus(const gp_Ax3& axis, float radius1, float radius2,
		float angle, float uParam, float vParam) :
		axis(axis), radius1(radius1), radius2(radius2),
		angle(angle), uParam(uParam), vParam(vParam)
	{
	}

	TopoDS_Shape CreateTorus::executeCreation() const
	{
		try
		{
			gp_Ax2 ax2(axis.Location(), axis.Direction());

			BRepPrimAPI_MakeTorus makeTorus(ax2, radius1, radius2, angle/180.0f*Standard_PI);
			return makeTorus.Shape();
		}
		catch(const StdFail_NotDone& ex)
		{
			throw Common::Exception(QObject::tr("Create torus error"));
		}
	}

}

