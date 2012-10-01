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

#include "cadapplication.h"

#include <Inventor/Qt/SoQt.h>

#include <mainwindow.h>

namespace Gui
{

	MainWindow* CADApplication::mainWindow = 0;

	CADApplication::CADApplication(int& argc, char* argv[]) :
		QApplication(argc, argv)
	{
		SoQt::init(argc, argv, argv[0]);
	}

	MainWindow* CADApplication::getMainWindow()
	{
		if (!mainWindow)
			mainWindow = new MainWindow();

		return mainWindow;
	}

}

