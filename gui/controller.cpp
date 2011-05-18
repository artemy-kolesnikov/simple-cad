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

#include "controller.h"

#include <model.h>
#include <command.h>

namespace Gui
{

	Controller::Controller(QObject* parent) : QObject(parent)
	{
	}

	void Controller::execCommand(Common::Command* cmd)
	{
		cmd->execute();
		delete cmd;
	}

	/*void Controller::updateView()
	{
		Q_EMIT updateViewRequest();
	}

	void Controller::loadModel(QString& fileName)
	{
		Q_EMIT loadModelRequest(fileName);
	}

	void Controller::saveModel(QString& fileName)
	{
		Q_EMIT saveModelRequest(fileName);
	}

	void Controller::setMaterial(Graphic3d_NameOfMaterial material)
	{
		Q_EMIT setMaterialRequest(material);
	}

	void Controller::setShadded(bool shadded)
	{
		Q_EMIT setShaddedRequest(shadded);
	}

	void Controller::createRectangle(gp_Pnt& pt, float width, float height)
	{
		Q_EMIT createRectangleRequest(pt, width, height);
	}

	void Controller::makePrism(AIS_Shape* shape, float height)
	{
		Q_EMIT makePrismRequest(shape, height);
	}*/

}

