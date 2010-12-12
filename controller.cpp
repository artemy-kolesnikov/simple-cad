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

#include "controller.h"

Controller::Controller(QObject* parent) : QObject(parent)
{
}

void Controller::updateView()
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

