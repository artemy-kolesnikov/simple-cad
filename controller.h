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

#ifndef CONTROOLER_HEADER
#define CONTROOLER_HEADER

#include <QObject>

#include <Graphic3d_NameOfMaterial.hxx>

/**
 * Controller dispatch commands between model and views set
 */
class Controller : public QObject
{
	Q_OBJECT

public:
	Controller(QObject* parent = 0);

public Q_SLOTS:
	void updateView();
	void loadModel(QString& fileName);
	void saveModel(QString& fileName);
	void setMaterial(Graphic3d_NameOfMaterial material);
	void setShadded(bool shadded);

Q_SIGNALS:
	void updateViewRequest();
	void loadModelRequest(QString& fileName);
	void saveModelRequest(QString& fileName);
	void setMaterialRequest(Graphic3d_NameOfMaterial material);
	void setShaddedRequest(bool shadded);
};

#endif // CONTROOLER_HEADER
