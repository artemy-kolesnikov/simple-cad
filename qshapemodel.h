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

#ifndef SHAPE_MODEL_HEADER
#define SHAPE_MODEL_HEADER

#include <QAbstractTableModel>

class Model;

class QShapeModel : public QAbstractTableModel
{
	Q_OBJECT
public:
    enum Roles {
		TypeRole = Qt::UserRole + 1
    };

	QShapeModel(Model* model, QObject *parent = 0);
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
	int columnCount(const QModelIndex &parent = QModelIndex()) const;
	int rowCount(const QModelIndex &parent = QModelIndex()) const;
	bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex());

private:
	Model* model;
};

#endif // SHAPE_MODEL_HEADER
