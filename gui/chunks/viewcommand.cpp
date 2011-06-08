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

#include "viewcommand.h"

#include <view.h>

namespace Common
{

	void ViewCommand::Execute()
	{
		switch (type)
		{
			case ViewFront:
				view->viewFront();
				break;
			case ViewBack:
				view->viewBack();
				break;
			case ViewTop:
				view->viewTop();
				break;
			case ViewBottom:
				view->viewBottom();
				break;
			case ViewLeft:
				view->viewLeft();
				break;
			case ViewRight:
				view->viewRight();
				break;
		}
	}

}

