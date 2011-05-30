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

#ifndef SKETCHER_VIEW_HEADER
#define SKETCHER_VIEW_HEADER

#include <interactiveview.h>
#include <sketchcommon.h>

#include <memory>
#include <Inventor/SbPlane.h>

class SbPlaneProjector;
class SbViewVolume;
class SoPath;
class SoTabBoxManip;

namespace Sketcher
{

	class CommandMessage;
	class Primitive2D;

	class SketcherView : public Gui::InteractiveView
	{
		Q_OBJECT
	public:
		SketcherView(SbPlane plane, QObject* parent = 0);
		~SketcherView();

		SbVec3f projectPoint(SbVec2f& curPos);
		SbVec3f projectPoint(SbVec2f& curPos, const SbViewVolume& volume);

		virtual void receive(Common::Message* msg);

	protected:
		virtual bool keyPressed(int key);
    	virtual bool mouseMove(const SbVec2f& pos, const SbVec3f& norm,
			const SoPickedPoint* pp);
    	virtual bool mouseButtonPressed(int Button, bool pressed, const SbVec2f &pos,
			const SbVec3f& norm, const SoPickedPoint* pp);
		virtual void pathSelected(SoPath*);
		virtual void pathDeselected(SoPath*);

	private Q_SLOTS:
		void primitiveCreated(Primitive2D* primitive);
		void primitiveCreationCanceled(Primitive2D* primitive);

	private:
		void drawGrid();
		void processMessage(CommandMessage* msg);
		void createPrimitive(PrimitiveType type);
		void resetCurrentPrimitive(bool removeFromScene);

	private:
		std::auto_ptr<SbPlaneProjector> planeProjector;
		SbPlane sketchPlane;

		Primitive2D* currentPrimitive;

		SoPath* xformPath;
		SoTabBoxManip* boxManip;
	};

}

#endif // SKETCHER_VIEW_HEADER

