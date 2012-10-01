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

#include "command.h"

#include <cassert>

namespace Common
{
	Command::Command() : QObject(),
		prepared(false)
	{
		setType(Immediately);
	}

	void Command::prepare()
	{
		doPrepare();
		prepared = true;
	}

	void Command::execute()
	{
		assert(!((type == Delayed) && !isReadyToExecute) && "Wrong prepare-execute call order");

		doExecute();
	}

	void Command::cancel()
	{
		doCancel();
		Q_EMIT canceled();
	}

	void Command::setType(Type type)
	{
		this->type = type;

		if (!prepared && type == Delayed)
			isReadyToExecute = false;
		else
			isReadyToExecute = true;
	}

	Command::Type Command::getType() const
	{
		return type;
	}

	void Command::emitReadyToExecute()
	{
		isReadyToExecute = true;
		Q_EMIT readyToExecute();
	}

}

