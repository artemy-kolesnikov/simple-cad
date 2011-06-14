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

#ifndef COMBINE_ACTION_HEADER
#define COMBINE_ACTION_HEADER

#include "topoaction.h"

#include <list>

namespace Action
{

	class CombineAction : public TopoAction
	{
	public:
		/**
		 * Shapes that should be removed after execution
		 */
		virtual std::list<TopoDS_Shape> getRemovedShapes() const = 0;
	};

}

#endif // COMBINE_ACTION_HEADER

