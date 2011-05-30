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

#ifndef INTERACTIVE_VIEW_HEADER
#define INTERACTIVE_VIEW_HEADER

#include <QObject>

#include <TopoDS_Shape.hxx>

#include <messagereceiver.h>

class SbVec2f;
class SbVec3f;
class SoEventCallback;
class SoPath;
class SoPickedPoint;
class SoQtViewer;
class SoSelection;
class SoSeparator;

namespace Gui
{

	class InteractiveView : public QObject, public Common::MessageReceiver
	{
		Q_OBJECT
	public:
		InteractiveView(QObject* parent = 0);
		virtual ~InteractiveView();

		SoSeparator* getSeparator() const;

		const SoQtViewer& getViewer() const;
		void setViewer(SoQtViewer* viewer);

	Q_SIGNALS:
		void shapeCreated(TopoDS_Shape& shape);

	protected:
		virtual void pathSelected(SoPath*) {}
		virtual void pathDeselected(SoPath*) {}
		virtual bool keyPressed(int key) = 0;
    	virtual bool mouseMove(const SbVec2f& pos, const SbVec3f& norm,
			const SoPickedPoint* pp) = 0;
    	virtual bool mouseButtonPressed(int Button, bool pressed, const SbVec2f &pos,
			const SbVec3f& norm, const SoPickedPoint* pp) = 0;

		void emitShapeCreated(TopoDS_Shape& shape);

	private:
		static void eventCallback(void* data, SoEventCallback* callback);
		static void selectionCallback(void* data, SoPath* path);
		static void deselectionCallback(void* data, SoPath* path);

	protected:
		SoSelection* rootSelection;
		SoSeparator* rootSeparator;
		SoSeparator* selectableSeparator;
		SoSeparator* unselectableSeparator;
		SoEventCallback* eventCallbacker;
		SoQtViewer* viewer;
	};

}

#endif // INTERACTIVE_VIEW_HEADER

