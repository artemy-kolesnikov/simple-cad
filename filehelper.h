/**********************************************************************
 *
 * Simple CAD System based on opencascade (www.opencascade.org)
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

#ifndef FILE_HELPER_HEADER
#define FILE_HELPER_HEADER

#include <QString>

#include <AIS_InteractiveContext.hxx>

/**
 * Import and export files in various formats
 */
class FileHelper
{
public:
	static bool readFile(QString& fileName,
		const Handle(AIS_InteractiveContext)& context);

	static bool writeFile(QString& fileName,
		const Handle(AIS_InteractiveContext)& context);
};

#endif // FILE_HELPER_HEADER

