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

#ifndef FILE_HELPER_HEADER
#define FILE_HELPER_HEADER

#include <QString>
#include <TopoDS_Shape.hxx>
#include <list>

#include "error.h"

/**
 * Import and export files in various formats
 */
class FileHelper
{
public:
	static void readFile(QString& fileName,
		std::list<TopoDS_Shape>& shapes);

	static void writeFile(QString& fileName,
		const std::list<TopoDS_Shape>& context);
};

#endif // FILE_HELPER_HEADER

