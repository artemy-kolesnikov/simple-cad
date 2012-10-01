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

#ifndef SEND_MESSAGE_COMMAND_HEADER
#define SEND_MESSAGE_COMMAND_HEADER

#include "command.h"
#include "messagereceiver.h"

namespace Common
{

	class SendMessageCommand : public Command
	{
	public:
		SendMessageCommand(MessageReceiver& receiver, Message* msg);

		virtual QString getName() const;

	private:
		virtual void doPrepare() {}
		virtual void doExecute();
		virtual void doCancel() {}

	private:
		MessageReceiver& receiver;
		Message* message;
	};

}

#endif // SEND_MESSAGE_COMMAND_HEADER

