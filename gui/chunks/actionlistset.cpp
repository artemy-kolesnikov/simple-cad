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

#include "actionlists.h"

#include "createactionlist.h"
#include "modifyactionlist.h"

namespace Gui
{

	ActionListSet::ActionListSet(QObject* parent) : QObject(parent),
		actionListSetCreated(false)
	{
	}

	void ActionListSet::getActionLists(QList<ActionList*>& acLists)
	{
		if (!actionListsCreated)
		{
			createActionListSet();
			actionListsCreated = true;
		}

		acLists = actionListSet;
	}


	void ActionListSet::createActionListSet()
	{
		CreateActionList* createActions = new CreateActionList(this);
		QList<ActionList*> createActionList;
		createActions->getActionList(createActionList);
		actionListSet.append(createActionList);

		ModifyActionList* modifyActions = new ModifyActionList(this);
		QList<ActionList*> modifyActionList;
		modifyActions->getActionList(modifyActionList);
		actionListSet.append(modifyActionList);
	}

}

#endif // ACTION_LISTS_HEADER
