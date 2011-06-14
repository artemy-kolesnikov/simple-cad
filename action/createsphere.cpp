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

#include "createsphere.h"

#include <exception.h>

#include <BRepPrimAPI_MakeSphere.hxx>
#include <StdFail_NotDone.hxx>
#include <gp_Ax2.hxx>

namespace Action
{

	CreateSphere::CreateSphere(const gp_Ax3& axis, float radius, float angle) :
		axis(axis), radius(radius), angle(angle)
	{
	}

	TopoDS_Shape CreateSphere::executeCreation() const
	{
		try
		{
			gp_Ax2 ax2(axis.Location(), axis.Direction());

			BRepPrimAPI_MakeSphere makeSphere(ax2, radius, angle/180.0f*Standard_PI);
			return makeSphere.Shape();
		}
		catch(const StdFail_NotDone& ex)
		{
			throw Common::Exception(QObject::tr("Create sphere error"));
		}
	}

}

