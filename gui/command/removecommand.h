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

#ifndef REMOVE_COMMAND_HEADER
#define REMOVE_COMMAND_HEADER

#include <command.h>

namespace Gui
{

	class Model;
	class ViewerShape;

	class RemoveCommand : public Common::Command
	{
	public:
		RemoveCommand(Model& model, const ViewerShape& shape);

		virtual QString getName() const;

	private:
		virtual void doPrepare() {}
		virtual void doExecute();
		virtual void doCancel() {}

	private:
		Model& model;
		const ViewerShape& shape;
	};

}

#endif // REMOVE_COMMAND_HEADER

