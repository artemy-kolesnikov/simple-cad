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

#include "removecommand.h"

#include <model.h>
#include <viewershape.h>

namespace Gui
{

	RemoveCommand::RemoveCommand(Model& model, const ViewerShape& shape) :
		model(model), shape(shape)
	{
	}

	void RemoveCommand::execute()
	{
		model.removeShape(shape.getShape());
	}

	QString RemoveCommand::getName() const
	{
		return QObject::tr("Удалить");
	}
}

