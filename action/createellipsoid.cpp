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

#include "createellipsoid.h"

#include <exception.h>

#include <BRepBuilderAPI_GTransform.hxx>
#include <BRepPrimAPI_MakeSphere.hxx>
#include <StdFail_NotDone.hxx>
#include <gp_Ax2.hxx>
#include <gp_Dir.hxx>

namespace Action
{

	CreateEllipsoid::CreateEllipsoid(const gp_Ax3& axis, float radius1, float radius2,
		float angle, float uParam, float vParam) :
		axis(axis), radius1(radius1), radius2(radius2),
		angle(angle), uParam(uParam), vParam(vParam)
	{
	}

	TopoDS_Shape CreateEllipsoid::executeCreation() const
	{
		try
		{
			gp_Pnt pnt(axis.Location());
			gp_Dir dir(axis.Direction());
			gp_Ax2 ax2(pnt, dir);

			BRepPrimAPI_MakeSphere makeSphere(ax2, radius2, angle/180.0f*Standard_PI);

			float scale = radius1 / radius2;
			gp_Dir xDir = ax2.XDirection();
			gp_Dir yDir = ax2.YDirection();
			gp_GTrsf mat;
			mat.SetValue(1, 1, xDir.X());
			mat.SetValue(2, 1, xDir.Y());
			mat.SetValue(3, 1, xDir.Z());
			mat.SetValue(1, 2, yDir.X());
			mat.SetValue(2, 2, yDir.Y());
			mat.SetValue(3, 2, yDir.Z());
			mat.SetValue(1, 3, dir.X()*scale);
			mat.SetValue(2, 3, dir.Y()*scale);
			mat.SetValue(3, 3, dir.Z()*scale);

			BRepBuilderAPI_GTransform trs(makeSphere.Shape(), mat);
			return trs.Shape();
		}
		catch(const StdFail_NotDone& ex)
		{
			throw Common::Exception(QObject::tr("Create ellipsoid error"));
		}
	}

}

