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

#include "createbox.h"

#include <exception.h>

#include <BRepPrimAPI_MakeBox.hxx>
#include <StdFail_NotDone.hxx>
#include <gp_Ax2.hxx>

#include <QDebug>

namespace Action
{

	CreateBox::CreateBox(const gp_Ax3& axis, float height, float width, float length) :
		axis(axis), height(height), width(width), length(length)
	{
	}

	TopoDS_Shape CreateBox::executeCreation() const
	{
		try
		{
			gp_Ax2 ax2(axis.Location(), axis.Direction());

			BRepPrimAPI_MakeBox makeBox(ax2, height, width, length);

			return makeBox.Shape();
		}
		catch(const StdFail_NotDone& ex)
		{
			throw Common::Exception(QObject::tr("Create box error"));
		}
	}

}
