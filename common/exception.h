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

#ifndef EXCEPTION_HEADER
#define EXCEPTION_HEADER

#include <exception>
#include <QString>

namespace Common
{

	class Exception : public std::exception
	{
	public:
		Exception(QString what) : whatStr(what) {}
		~Exception() throw() {}

		const char* what() { return whatStr.toStdString().c_str(); }

	private:
		QString whatStr;
	};

	class FileException : public Exception
	{
	public:
		FileException(QString what) : Exception(what) {}
	};

}

#endif // EXCEPTION_HEADER

