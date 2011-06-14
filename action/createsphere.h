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

#ifndef CREATE_SPHERE_HEADER
#define CREATE_SPHERE_HEADER

#include <gp_Ax3.hxx>
#include <TopoDS_Shape.hxx>

#include <QObject>

#include "create3dprimitive.h"

namespace Action
{

	class CreateSphere : public Create3dPrimitive
	{
	public:
		CreateSphere(const gp_Ax3& axis, float radius, float angle);

		virtual QString getName() const
		{
			return QObject::tr("Create sphere");
		}

	private:
		virtual TopoDS_Shape executeCreation() const;

	private:
		gp_Ax3 axis;
		float radius;
		float angle;
	};

}

#endif // CREATE_SPHERE_HEADER

