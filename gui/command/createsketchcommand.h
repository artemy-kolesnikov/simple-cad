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

#ifndef CREATE_SKETCH_COMMAND_HEADER
#define CREATE_SKETCH_COMMAND_HEADER

#include <Inventor/SbPlane.h>

#include <command.h>

namespace Gui
{

	class View;

	class CreateSketchCommand : public Common::Command
	{
	public:
		CreateSketchCommand(SbPlane sketchPlane, View& view) :
			sketchPlane(sketchPlane), view(view)
		{
		}

		virtual QString getName() const;

	private:
		virtual void doPrepare() {}
		virtual void doExecute();
		virtual void doCancel() {}

	private:
		SbPlane sketchPlane;
		View& view;
	};

}

#endif // CREATE_SKETCH_COMMAND_HEADER

