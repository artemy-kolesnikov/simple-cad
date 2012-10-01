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

#include "fuse.h"

#include <BRepAlgoAPI_Fuse.hxx>
#include <StdFail_NotDone.hxx>
#include <Standard_ConstructionError.hxx>

#include <exception.h>

namespace Action
{

	Fuse::Fuse(const TopoDS_Shape& shape1, const TopoDS_Shape& shape2) :
		Boolean(shape1, shape2)
	{
	}

	TopoDS_Shape Fuse::execute(const TopoDS_Shape& shape1, const TopoDS_Shape& shape2) const
	{
		try
		{
			BRepAlgoAPI_Fuse fuse(shape1, shape2);
			return fuse.Shape();
		}
		catch(const StdFail_NotDone& ex)
		{
			throw Common::Exception(QObject::tr("Ошибка выполнения операции"));
		}
		catch(const Standard_ConstructionError&)
		{
			throw Common::Exception(QObject::tr("Невозможно создать фигуру"));
		}
	}

}
