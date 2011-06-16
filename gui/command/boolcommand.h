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
	class View;
	class BoolOperationWidget;
	class PickShapeWidget;

	class BooleanCommand : public Common::Command
	{
		Q_OBJECT
	public:
		enum OperationType
		{
			Fuse,
			Common,
			Cut
		};

		BooleanCommand(Model& model, View& view, OperationType type);
		~BooleanCommand();

		virtual QString getName() const;
		static QString getOperationNameByType(OperationType type);

	private:
		virtual void doPrepare();
		virtual void doExecute();
		virtual void doCancel();

	private:
		Model& model;
		View& view;
		OperationType type;
		BoolOperationWidget* operationWidget;
		PickShapeWidget* pickWidget;
	};
}

#endif // BOOL_COMMAND_HEADER

