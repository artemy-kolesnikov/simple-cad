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

#ifndef WIDGET_DIALOG_HEADER
#define WIDGET_DIALOG_HEADER

#include <QDialog>

class QVBoxLayout;

class WidgetDialog : public QDialog
{
	Q_OBJECT
public:
	WidgetDialog(QWidget* parent = 0);

	void setCentralWidget(QWidget* widget);
	QWidget* getCentralWidget() const;

private:
	void createUI();

	QVBoxLayout* layout;
	QWidget* centralWidget;
};

#endif // WIDGET_DIALOG_HEADER

