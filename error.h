/*
 * Simple CAD System
 * Based on opencascade (www.opencascade.org)
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

#ifndef ERROR_HEADER
#define ERROR_HEADER

#include <QString>
#include <exception>

class CadError : public std::exception
{
public:
	CadError(QString what) : whatStr(what) {}
	~CadError() throw() {}

	const char* what() { return whatStr.toStdString().c_str(); }

private:
	QString whatStr;
};

class FileError : public CadError
{
public:
	FileError(QString what) : CadError(what) {}
};

#endif // ERROR_HEADER

