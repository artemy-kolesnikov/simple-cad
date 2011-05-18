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

#ifndef MODEL_HEADER
#define MODEL_HEADER

#include <QObject>
#include <QStringList>

#include <gp_Pnt.hxx>
#include <TopTools_HSequenceOfShape.hxx>

#include <list>
#include <map>
#include <boost/shared_ptr.hpp>

#include "error.h"

namespace Gui
{

	/**
	 * Keeps loaded CAD model
	 */
	class Model : public QObject
	{
		Q_OBJECT

	public:
		Model(QObject* parent = 0);

		QString getFileName() const;

		const TopTools_HSequenceOfShape& getShapes() const;

		void removeShape(const TopoDS_Shape& shape);
		void addShape(const TopoDS_Shape& shape);

	Q_SIGNALS:
		void shapeAdded(const TopoDS_Shape& shape);
		void shapeRemoved(const TopoDS_Shape& shape);
		void fileNameChanged(QString& newFileName);

	public Q_SLOTS:
		void loadModel(QString& fileName);
		void saveModel(QString& fileName);

	private:
		QString fileName;

		Handle(TopTools_HSequenceOfShape) shapeList;
	};

}

#endif // MODEL_HEADER

