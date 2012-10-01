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

#include "controller.h"

#include <model.h>
#include <command.h>

#include <cassert>

namespace Gui
{

	Controller::Controller(QObject* parent) : QObject(parent), currentCommand(0)
	{
	}

	void Controller::execCommand(Common::Command* cmd)
	{
		assert(!currentCommand && "Execute invoked before previous opearion completion");

		cmd->prepare();

		if (cmd->getType() == Common::Command::Delayed)
		{
			currentCommand = cmd;
			connect(cmd, SIGNAL(readyToExecute()), this, SLOT(readyToExecute()));
			connect(cmd, SIGNAL(canceled()), this, SLOT(canceled()));
		}
		else
		{
			cmd->execute();
			delete cmd;
		}
	}

	void Controller::readyToExecute()
	{
		currentCommand->execute();
		delete currentCommand;
		currentCommand = 0;
	}

	void Controller::canceled()
	{
		delete currentCommand;
		currentCommand = 0;
	}

	void Controller::cancelAllCommands()
	{
		currentCommand->cancel();
	}

}

