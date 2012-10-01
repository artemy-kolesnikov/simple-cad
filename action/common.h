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

#ifndef COMMON_ACTION_HEADER
#define COMMON_ACTION_HEADER

#include <TopoDS_Shape.hxx>

#include <QObject>

#include "boolean.h"

namespace Action
{

	class Common : public Boolean
	{
	public:
		Common(const TopoDS_Shape& shape1, const TopoDS_Shape& shape2);

		virtual QString getName() const
		{
			return QObject::tr("Common");
		}

	protected:
		virtual TopoDS_Shape execute(const TopoDS_Shape& shape1, const TopoDS_Shape& shape2) const;
	};

}

#endif // COMMON_ACTION_HEADER

