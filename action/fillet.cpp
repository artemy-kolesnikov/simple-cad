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

#include "fillet.h"

#include <BRepFilletAPI_MakeFillet.hxx>
#include <StdFail_NotDone.hxx>
#include <TopExp_Explorer.hxx>
#include <TopoDS.hxx>
#include <Standard_Failure.hxx>

#include <common/exception.h>

namespace Action
{
	Fillet::Fillet(TopoDS_Shape shape, TopoDS_Shape edge, float radius) :
		shape(shape), edge(edge), radius(radius)
	{
	}

	TopoDS_Shape Fillet::getShape() const
	{
		return resShape;
	}

	void Fillet::execute()
	{
		BRepFilletAPI_MakeFillet makeFillet(shape);

		/*TopExp_Explorer explorer(shape, TopAbs_EDGE);
		while (explorer.More())
		{
			makeFillet.Add(radius, TopoDS::Edge(explorer.Current()));
			explorer.Next();
		}*/

		try
		{
			makeFillet.Add(radius, TopoDS::Edge(edge));
			resShape = makeFillet.Shape();
		}
		catch(const StdFail_NotDone& ex)
		{
			QString msg(QObject::tr("Ошибка выполнения операции"));
			msg += "\n";
			msg += ex.GetMessageString();
			throw Common::Exception(msg);
		}
		catch(const Standard_Failure& ex)
		{
			QString msg(QObject::tr("Ошибка создания фигуры"));
			msg += "\n";
			msg += ex.GetMessageString();
			throw Common::Exception(msg);
		}
	}

	std::list<TopoDS_Shape> Fillet::getRemovedShapes() const
	{
		std::list<TopoDS_Shape> shapes;
		shapes.push_back(shape);
		return shapes;
	}
}
