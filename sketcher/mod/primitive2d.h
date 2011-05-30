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

#ifndef PRIMITIVE2D_HEADER
#define PRIMITIVE2D_HEADER

#include <Inventor/SbVec3f.h>
#include <Inventor/nodes/SoSeparator.h>

#include <memory>

#include <QObject>

class SoField;
class SoFieldSensor;
class SoQtRenderArea;
class SoSensor;

namespace Sketcher
{

	class Primitive2D;
	class SketcherView;

	class Primitive2DEvents : public QObject
	{
		Q_OBJECT
		friend class Primitive2D;
	private:
		Primitive2DEvents() {}

	Q_SIGNALS:
		void created(Primitive2D* primitive);
		void creationCanceled(Primitive2D* primitive);
	};

	class Primitive2D : public SoSeparator
	{
	public:
		Primitive2D(SketcherView& sketcherView);
		virtual ~Primitive2D() {}

		void handleEvents(bool handle);
		bool isEventsHandles() const;

		Primitive2DEvents* getEvents() const;
	protected:
		virtual bool keyPressed(int key) = 0;
    	virtual bool mouseMove(const SbVec3f& pos) = 0;
    	virtual bool mouseButtonPressed(int Button, bool pressed, const SbVec3f &pos) = 0;

		void attachFieldSensor(SoField* field);
		virtual void fieldSensorCallback(SoSensor*) {}

		void emitCreated();
		void emitCreationCanceled();

	private:
		virtual void handleEvent(SoHandleEventAction*);
		static void fieldSensorCallback(void* userData, SoSensor* sensor);

	protected:
		SketcherView& sketcherView;
	private:
		std::auto_ptr<SoFieldSensor> coordFieldSensor;
		SoHandleEventAction* eventAction;
		bool eventsHandles;

		std::auto_ptr<Primitive2DEvents> events;
	};

}

#endif // PRIMITIVE2D_HEADER

