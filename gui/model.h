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
#include <QList>

#include <list>
#include <map>
#include <memory>

#include <shape.h>

namespace Gui
{

	class ShapeModel;

	/**
	 * Keeps loaded CAD model
	 */
	class Model : public QObject
	{
		Q_OBJECT

	public:
		Model(QObject* parent = 0);
		~Model();

		QString getFileName() const;

		const QList<Shape>& getShapes() const;

		void removeShape(const Shape& shape);
		void addShape(const Shape& shape);

		ShapeModel* getQModel() const;

		void test();

	Q_SIGNALS:
		void shapeAdded(const Shape& shape);
		void shapeRemoved(const Shape& shape);
		void fileNameChanged(QString& newFileName);

	public Q_SLOTS:
		void loadModel(QString& fileName);
		void saveModel(QString& fileName);

	private:
		QString fileName;

		QList<Shape> shapeList;

		std::auto_ptr<ShapeModel> shapeModel;
	};

}

#endif // MODEL_HEADER

