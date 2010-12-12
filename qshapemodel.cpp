#include "qshapemodel.h"
#include "model.h"

#include <TopTools_HSequenceOfShape.hxx>

namespace
{
	QString header_data[1] = { QObject::tr("Type") };
}

QShapeModel::QShapeModel(Model* model, QObject *parent) : QAbstractTableModel(parent)
{
}

QVariant QShapeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
		return header_data[section];

	return QAbstractTableModel::headerData(section, orientation, role);
}

QVariant QShapeModel::data(const QModelIndex &index, int role) const
{
	Handle(TopTools_HSequenceOfShape) shapes = model->getShapes();

	if (index.row() < 0 || index.row() >= shapes->Length())
		return QVariant();

	switch (role)
	{
	case TypeRole:
		return QString(tr("Shape_%d").arg(index.row()));
	case Qt::DisplayRole:
	case Qt::EditRole:
	{
		switch (index.column())
		{
		case 0:
			return QString("EditRole oops column 0");
		case 1:
			return QString("EditRole oops column 1");
		}
	}
	case Qt::DecorationRole:
		if (index.column() == 0)
		{
			return QString("DecorationRole oops column 0");
		}
	}

	return QVariant();
}

int QShapeModel::columnCount(const QModelIndex &parent) const
{
	return (parent.isValid()) ? 0 : 1;
}

int QShapeModel::rowCount(const QModelIndex &parent) const
{
	return (parent.isValid()) ? 0 : model->getShapes()->Length();
}

bool QShapeModel::removeRows(int row, int count, const QModelIndex &parent)
{
	if (parent.isValid())
		return false;

	return true;
}

