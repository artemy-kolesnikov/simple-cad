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

#ifndef COMMAND_HEADER
#define COMMAND_HEADER

#include <QString>
#include <QObject>

namespace Common
{

	class Command : public QObject
	{
		Q_OBJECT
	public:
		enum  Type
		{
			Immediately,
			Delayed
		};

		Command();
		virtual ~Command() {}

		virtual QString getName() const = 0;

		Type getType() const;

	public Q_SLOTS:
		void prepare();
		void execute();
		void cancel();

	protected Q_SLOTS:
		void emitReadyToExecute();

	protected:
		void setType(Type type);

	private:
		virtual void doPrepare() = 0;
		virtual void doExecute() = 0;
		virtual void doCancel() = 0;

	Q_SIGNALS:
		/**
		 * Emited for delayed command type
		 */
		void readyToExecute();
		/**
		 * Emited for delayed command type
		 */
		void canceled();

	private:
		Type type;
		bool isReadyToExecute;
		bool prepared;
	};

}

#endif // COMMAND_HEADER

