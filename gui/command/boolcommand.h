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

#ifndef BOOL_COMMAND_HEADER
#define BOOL_COMMAND_HEADER

#include <command.h>
#include <QString>

namespace Gui
{

	class Model;

	class BooleanCommand : public Common::Command
	{
	public:
		enum OperationType
		{
			Fuse,
			Common,
			Cut
		};

		QString getOperationName() const;
		static QString getOperationNameByType(OperationType type);

		BooleanCommand(Model& model, OperationType type);
		virtual void execute();

	private:
		Model& model;
		OperationType type;
	};
}

#endif // BOOL_COMMAND_HEADER

