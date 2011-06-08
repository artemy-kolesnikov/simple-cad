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

#ifndef MULTIDOC_CONTAINER_HEADER
#define MULTIDOC_CONTAINER_HEADER

#include <QList>
#include <QAction>

class Model;

namespace Gui
{

	class View;

	class MultiDocContainer
	{
	public:
		virtual ~MultiDocContainer();

		virtual Model* getActiveModel() const = 0;
		virtual View* getActiveView() const = 0;
	};

}

#endif // MULTIDOC_CONTAINER_HEADER

