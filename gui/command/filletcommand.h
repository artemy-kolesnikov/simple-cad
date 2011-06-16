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

#ifndef FILLET_HEADER_FILE
#define FILLET_HEADER_FILE

#include <command.h>

namespace Gui
{

	class Model;
	class View;
	class ViewerShape;
	class FilletOperationWidget;
	class PickShapeWidget;

	class FilletCommand : public Common::Command
	{
		Q_OBJECT
	public:
		FilletCommand(Model& model, View& view, const ViewerShape& shape);
		~FilletCommand();

		virtual QString getName() const;

	private Q_SLOTS:
		void confirmed();

	private:
		virtual void doPrepare();
		virtual void doExecute();
		virtual void doCancel();

	private:
		Model& model;
		View& view;
		const ViewerShape& shape;
		FilletOperationWidget* operationWidget;
		PickShapeWidget* pickWidget;
	};

}

#endif // FILLET_HEADER_FILE

