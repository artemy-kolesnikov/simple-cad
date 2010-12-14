#include "qshapemodel.h"
#include "model.h"

#include <TopTools_HSequenceOfShape.hxx>

namespace
{
	QString header_data[1] = { QObject::tr("Type") };

	const QString root_title = QObject::tr("Shapes");

	/*class ModelAdapter
	{
	public:
		Handle(TopTools_HSequenceOfShape) shapes;
	};*/
}

QShapeModel::QShapeModel(Model* model, QObject *parent) : QAbstractItemModel(parent),
	model(model)
{
}

QVariant QShapeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (Qt::Horizontal == orientation && Qt::DisplayRole == role)
		return header_data[section];

	return QAbstractItemModel::headerData(section, orientation, role);
}

QVariant QShapeModel::data(const QModelIndex &index, int role) const
{
	Handle(TopTools_HSequenceOfShape) shapes = model->getShapes();

	if (index.row() < 0 || index.row() >= shapes->Length())
		return QVariant();

	if (Qt::DisplayRole != role)
		return QVariant();

	if (!index.parent().isValid())
		return root_title;

	return QString(tr("Shape_%d").arg(index.row()));
}

QModelIndex QShapeModel::index(int row, int column, const QModelIndex & parent) const
{
	if (row < 0 || column < 0 || column >= columnCount(parent))
			//|| parent.column() > 0)
		return QModelIndex();

	if (parent.isValid())
		return QModelIndex();

	return createIndex(row, column, row);

	/*if (!hasIndex(row, column, parent))
		return QModelIndex();

	if (parent.isValid())
	{
		Handle(TopTools_HSequenceOfShape) shapes = model->getShapes();
		return createIndex(row, column, &shapes->Value(row));
	}
	else
		return createIndex(row, column, &root_title);*/
}

int QShapeModel::columnCount(const QModelIndex &parent) const
{
	return 2;
	return (parent.isValid()) ? 1 : 2;
}

int QShapeModel::rowCount(const QModelIndex &parent) const
{
	return 10;
	int res = (parent.isValid()) ? 10 : model->getShapes()->Length();
	return res;
}

bool QShapeModel::removeRows(int row, int count, const QModelIndex &parent)
{
	if (parent.isValid())
		return false;

	return true;
}

bool QShapeModel::removeRow(int row, const QModelIndex &parent)
{
	if (parent.isValid())
		return false;

	return true;
}

QModelIndex	QShapeModel::parent(const QModelIndex &index) const
{
	int offset = index.internalId();

	if (offset == 0 || !index.isValid())
		return QModelIndex();

	return createIndex(0, 0, 0);
}

Qt::ItemFlags QShapeModel::flags(const QModelIndex &index) const
{
	if (!index.isValid())
		return Qt::NoItemFlags;

	return Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsDragEnabled;
}

