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

#ifndef CREATE_PRIMITIVE_COMMAND_HEADER
#define CREATE_PRIMITIVE_COMMAND_HEADER

#include <command.h>
#include <QStringList>

namespace Gui
{

	class Model;

	class CreatePrimitiveCommand : public Common::Command
	{
	public:
		enum ObjectType
		{
			Rectangle,
			Circle,
			Plane,
			Box,
			Cylinder,
			Cone,
			Sphere,
			Ellipsoid,
			Torus
		};

		static QStringList getObjectNames();

		CreatePrimitiveCommand(Model& model, ObjectType type) :
			model(model), type(type)
		{
		}

		virtual void execute();

	private:
		void createRectangle();
		void createCircle();
		void createBox();
		void createCylinder();
		void createSphere();
		void createCone();
		void createTorus();
		void createPlane();
		void createEllipsoid();

	private:
		Model& model;
		ObjectType type;
	};

}

#endif // CREATE_PRIMITIVE_COMMAND_HEADER

