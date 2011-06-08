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

#ifndef ACTION_LIST_HEADER
#define ACTION_LIST_HEADER

#include <QObject>
#include <QList>
#include <QAction>
#include <QString>

namespace Gui
{

	/**
	 * Action list. Used to group similar actions
	 */
	class ActionList : public QObject
	{
	public:
		ActionList(QObject* parent = 0) : QObject(parent), actionsCreated(false) {}
		virtual ~ActionList() = 0;

		void getActionList(QList<QAction*> >& list)
		{
			if (!actionsCreated)
			{
				createActions();
				actionsCreated = true;
			}

			list = actionList;
		}

		virtual QString getName() const = 0;

	protected:
		QList<QAction*> actionList;

	private:
		virtual void createActions() = 0;

		bool actionsCreated;
	};

}

#endif // ACTION_LIST_HEADER

