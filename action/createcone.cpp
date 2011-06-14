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

#include "createcone.h"

#include <exception.h>

#include <BRepPrimAPI_MakeCone.hxx>
#include <StdFail_NotDone.hxx>
#include <gp_Ax2.hxx>

namespace Action
{

	CreateCone::CreateCone(const gp_Ax3& axis, float radius1,
		float radius2, float height, float angle) :
		axis(axis), radius1(radius1), radius2(radius2),
		height(height), angle(angle)
	{
	}

	TopoDS_Shape CreateCone::executeCreation() const
	{
		try
		{
			gp_Ax2 ax2(axis.Location(), axis.Direction());

			BRepPrimAPI_MakeCone makeCone(ax2, radius1, radius2, height, angle/180.0f*Standard_PI);

			return makeCone.Shape();
		}
		catch(const StdFail_NotDone& ex)
		{
			throw Common::Exception(QObject::tr("Create cone error"));
		}
	}

}

