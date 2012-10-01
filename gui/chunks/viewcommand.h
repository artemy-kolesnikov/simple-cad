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

#ifndef VIEW_COMMAND_HEADER
#define VIEW_COMMAND_HEADER

#include <command.h>

namespace Gui
{
	class View;
}

namespace Common
{

	class ViewCommand : public Command
	{
	public:
		enum CommandType
		{
			ViewFront,
			ViewBack,
			ViewTop,
			ViewBottom,
			ViewLeft,
			ViewRight
		};

		ViewCommand(View* view, CommandType type) : view(view), type(type)
		{
		}

		virtual void Execute();

	private:
		ViewCommandType type;
		Gui::View* view;
	};

}

#if 0
	void Model::test()
	{
		//TopoDS_Shape shape1 = shapeList.at(0).getShape();
		//TopoDS_Shape shape2 = shapeList.at(1).getShape();

		gp_Pnt pnt;
		gp_Dir dir1(gp::DX());
		gp_Ax3 axis1(pnt, dir1);

		/*Action::CreateBox* createBox = new Action::CreateBox(axis, 10, 10, 10);
		createBox->execute();
		TopoDS_Shape box = createBox->getShape();*/

		/*Action::CreateCone* createCone = new Action::CreateCone(axis1, 10, 5, 10, 180);
		createCone->execute();
		TopoDS_Shape cone = createCone->getShape();*/

		gp_Ax2 ax1(axis1.Location(), axis1.Direction());
		//BRepPrimAPI_MakeCone makeCone(ax1, 10, 5, 10, Standard_PI);
		//TopoDS_Shape cone = makeCone.Shape();
		TopoDS_Solid cone = BRepPrimAPI_MakeCone(ax1, 10, 5, 10, Standard_PI);

		gp_Dir dir2(gp::DX());
		gp_Ax3 axis2(pnt, dir2);

		/*Action::CreateCylinder* createCylinder = new Action::CreateCylinder(axis2, 5, 10, 360);
		createCylinder->execute();
		TopoDS_Shape cylinder = createCylinder->getShape();*/

		gp_Ax2 ax2(axis2.Location(), axis2.Direction());
		//BRepPrimAPI_MakeCylinder makeCylinder(ax2, 5, 10);
		//TopoDS_Shape cylinder = makeCylinder.Shape();
		TopoDS_Solid cylinder = BRepPrimAPI_MakeCylinder(ax2, 5, 10);

		TopoDS_Shape shape = BRepAlgoAPI_Fuse(cone, cylinder);
		//TopoDS_Shape shape = common.Shape();

		//addShape(Shape(cylinder));
		//addShape(Shape(cone));
		addShape(Shape(shape));
	}

#endif
#endif // VIEW_COMMAND_HEADER

