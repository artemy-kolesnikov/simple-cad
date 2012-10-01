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

#ifndef CONTROOLER_HEADER
#define CONTROOLER_HEADER

#include <QObject>

//#include <Graphic3d_NameOfMaterial.hxx>
//#include <gp_Pnt.hxx>

namespace Common
{
	class Command;
}

class Model;

namespace Gui
{

	/**
	 * Controller send commands to active model
	 */
	class Controller : public QObject
	{
		Q_OBJECT

	public:
		Controller(QObject* parent = 0);

		void execCommand(Common::Command* cmd);

	public Q_SLOTS:
		void cancelAllCommands();

	private Q_SLOTS:
		void readyToExecute();
		void canceled();

	private:
		Common::Command* currentCommand;
	};

}

#endif // CONTROOLER_HEADER

