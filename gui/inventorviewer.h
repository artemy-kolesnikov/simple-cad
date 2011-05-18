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

#ifndef INVENTOR_VIEWER_HEADER
#define INVENTOR_VIEWER_HEADER

#include <Inventor/Qt/viewers/SoQtExaminerViewer.h>

#include <QWidget>

class SoNode;
class SoSelection;

namespace Gui
{

	class InteractiveView;

	class InventorViewer : public SoQtExaminerViewer
	{
		typedef SoQtExaminerViewer inherited;
	public:
		InventorViewer(QWidget* parent = 0);
		~InventorViewer();

		void setInteractiveView(InteractiveView* view);
		InteractiveView* getInteractiveView() const;

		SoSelection* getRootNode();

	private:
		virtual void setSceneGraph(SoNode* root);
		virtual SoNode* getSceneGraph();

	private:
		InteractiveView* interactiveView;

		SoSelection* rootSelection;
	};

}

#endif // INVENTOR_VIEWER_HEADER

