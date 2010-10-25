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

#include <QTextCodec>
#include <QPlastiqueStyle>

#include <locale.h>

#include "mainwindow.h"
#include "cadapplication.h"

int main(int argc, char *argv[])
{
	CADApplication a(argc, argv);

	CADApplication::setStyle(new QPlastiqueStyle());

	setlocale(LC_ALL, "rus_rus");
	setlocale(LC_NUMERIC, "English");
	setlocale(LC_TIME, "English");

	QTextCodec::setCodecForTr(QTextCodec::codecForName("utf-8"));
	QTextCodec::setCodecForLocale(QTextCodec::codecForName("utf-8"));
	QTextCodec::setCodecForCStrings(QTextCodec::codecForName("utf-8"));

	MainWindow w;
	w.show();

	return a.exec();
}

