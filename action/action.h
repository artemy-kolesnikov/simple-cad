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

#ifndef ACTION_HEADER
#define ACTION_HEADER

namespace Action
{

	/**
	 * Base action
	 */
	class Action
	{
	public:
		virtual ~Action() {}

		virtual void execute() = 0;
		virtual QString getName() const = 0;
	};

}

#endif // ACTION_HEADER

