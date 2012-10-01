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

#include "common.h"

#include <BRepAlgoAPI_Common.hxx>
#include <StdFail_NotDone.hxx>
#include <Standard_ConstructionError.hxx>

#include <exception.h>

namespace Action
{

	Common::Common(const TopoDS_Shape& shape1, const TopoDS_Shape& shape2) :
		Boolean(shape1, shape2)
	{
	}

	TopoDS_Shape Common::execute(const TopoDS_Shape& shape1, const TopoDS_Shape& shape2) const
	{
		try
		{
			BRepAlgoAPI_Common common(shape1, shape2);
			TopoDS_Shape shape = common.Shape();
			return shape;
		}
		catch(const StdFail_NotDone& ex)
		{
			throw ::Common::Exception(QObject::tr("Ошибка выполнения операции"));
		}
		catch(const Standard_ConstructionError&)
		{
			throw ::Common::Exception(QObject::tr("Невозможно создать фигуру"));
		}
	}

}
