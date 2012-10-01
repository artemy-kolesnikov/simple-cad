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

#ifndef ACTION_LISTSET_HEADER
#define ACTION_LISTSET_HEADER

#include <QList>
#include <QObject>

#include "actionlist.h"

namespace Gui
{

	/**
	 * Action list set for main window
	 */
	class ActionListSet : public QObject
	{
	public:
		ActionListSet(QObject* parent = 0);

		void getActionListSet(QList<ActionList*>& acLists);

	private:
		void createActionListSet();

	private:
		bool ActionListSetCreated;
		QList<ActionList*> actionListSet;
	};

}

#endif // ACTION_LISTSET_HEADER

