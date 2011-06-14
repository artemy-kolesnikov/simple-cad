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

#include "createrectangle.h"

#include <exception.h>

#include <BRepBuilderAPI_MakeEdge.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <BRepBuilderAPI_MakeWire.hxx>
#include <GC_MakeSegment.hxx>
#include <Geom_TrimmedCurve.hxx>
#include <StdFail_NotDone.hxx>
#include <TopoDS_Edge.hxx>
#include <TopoDS_Wire.hxx>
#include <gp_Pnt.hxx>

namespace Action
{

	CreateRectangle::CreateRectangle(const gp_Pnt& point, float width, float height) :
		point(point), width(width), height(height)
	{
	}

	TopoDS_Shape CreateRectangle::executeCreation() const
	{
		try
		{
			gp_Pnt pt1(point);
			gp_Pnt pt2(point.X() + width, point.Y(), point.Z());
			gp_Pnt pt3(point.X() + width, point.Y() + height, point.Z());
			gp_Pnt pt4(point.X(), point.Y() + height, point.Z());

			Handle(Geom_TrimmedCurve) segment1 = GC_MakeSegment(pt1, pt2);
			Handle(Geom_TrimmedCurve) segment2 = GC_MakeSegment(pt2, pt3);
			Handle(Geom_TrimmedCurve) segment3 = GC_MakeSegment(pt3, pt4);
			Handle(Geom_TrimmedCurve) segment4 = GC_MakeSegment(pt4, pt1);

			TopoDS_Edge edge1 = BRepBuilderAPI_MakeEdge(segment1);
			TopoDS_Edge edge2 = BRepBuilderAPI_MakeEdge(segment2);
			TopoDS_Edge edge3 = BRepBuilderAPI_MakeEdge(segment3);
			TopoDS_Edge edge4 = BRepBuilderAPI_MakeEdge(segment4);

			TopoDS_Wire wire  = BRepBuilderAPI_MakeWire(edge1 , edge2 , edge3, edge4);

			BRepBuilderAPI_MakeFace makeFace(wire);
			return makeFace.Shape();
		}
		catch(const StdFail_NotDone& ex)
		{
			throw Common::Exception(QObject::tr("Create rectangle error"));
		}
	}

}

