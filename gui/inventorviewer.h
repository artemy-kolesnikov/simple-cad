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
class SoEventCallback;

namespace Gui
{

	class InteractiveView;
	class InventorViewer;

	class ViewerEvents : public QObject
	{
		friend class InventorViewer;

	private:
		ViewerEvents() {}

	Q_SIGNALS:
	};

	class InventorViewer : public SoQtExaminerViewer
	{
		typedef SoQtExaminerViewer inherited;
	public:
		InventorViewer(QWidget* parent = 0);
		~InventorViewer();

		void setInteractiveView(InteractiveView* view);
		InteractiveView* getInteractiveView() const;

		SoSelection* getRootNode();

		void setCameraOrientation(const SbRotation& rotation);

	private:
		virtual void setSceneGraph(SoNode* root);
		virtual SoNode* getSceneGraph();

		static void eventCallback(void* data, SoEventCallback* callback);
		static void selectionCallback(void* data, SoPath* path);
		static void deselectionCallback(void* data, SoPath* path);

		void eventHandler(SoEventCallback* callback);
		void selectionHandler(SoPath* path);
		void deselectionHandler(SoPath* path);

	private:
		InteractiveView* interactiveView;

		SoSelection* rootSelection;
		SoEventCallback* eventCallbacker;
	};

}

#endif // INVENTOR_VIEWER_HEADER

