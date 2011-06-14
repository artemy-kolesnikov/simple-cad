#include "shapemodel.h"

#include <shape.h>

#include <QList>
#include <QDebug>

#include "model.h"

namespace Gui
{

	namespace _Impl
	{

		class Item
		{
		public:
			Item(QList<QVariant>& data, Item* parent = 0);
			~Item();

			void appendChild(Item* child);
			Item* child(int row) const;
			int childCount() const;
			int columnCount() const;
			QVariant data(int column) const;
			int row() const;
			Item* parent() const;
			void removeAll();

		private:
			QList<Item*> childItems;
			QList<QVariant> itemData;
			Item* itemParent;
		};


		Item::Item(QList<QVariant>& data, Item* parent)
		{
			itemParent = parent;
			itemData = data;
		}

		Item::~Item()
		{
			removeAll();
		}

		void Item::appendChild(Item* child)
		{
			childItems.append(child);
		}

		Item* Item::child(int row) const
		{
			return childItems.value(row);
		}

		int Item::childCount() const
		{
			return childItems.count();
		}

		int Item::columnCount() const
		{
			return itemData.count();
		}

		QVariant Item::data(int column) const
		{
			return itemData.value(column);
		}

		int Item::row() const
		{
			if (itemParent)
				return itemParent->childItems.indexOf(const_cast<Item*>(this));

			return 0;
		}

		Item* Item::parent() const
		{
			return itemParent;
		}

		void Item::removeAll()
		{
			qDeleteAll(childItems);
			Item* item;
			foreach(item, childItems)
				item = 0;
		}

	}

	using namespace _Impl;

	ShapeModel::ShapeModel(Model* model) : QAbstractItemModel(model), model(model)
	{
		QList<QVariant> rootData;
		rootData << tr("Объекты");
		rootItem.reset(new Item(rootData));

		//connect(model, SIGNAL(shapeAdded(const Shape&)),
			//this, SLOT(shapeAdded(const Shape&)));

		//connect(model, SIGNAL(shapeRemoved(const Shape&)),
			//this, SLOT(shapeRemoved(const Shape&)));
	}

	ShapeModel::~ShapeModel()
	{
	}

	QVariant ShapeModel::data(const QModelIndex &index, int role) const
	{
		if (!index.isValid())
			return QVariant();

		if (role != Qt::DisplayRole)
			return QVariant();

		Item* item = static_cast<Item*>(index.internalPointer());

		return item->data(index.column());
	}

	Qt::ItemFlags ShapeModel::flags(const QModelIndex &index) const
	{
		if (!index.isValid())
			return Qt::ItemIsEnabled;

		return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
	}

	QVariant ShapeModel::headerData(int section, Qt::Orientation orientation, int role) const
	{
		if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
			return rootItem->data(section);

		return QVariant();
	}

	QModelIndex ShapeModel::index(int row, int column,
					const QModelIndex &parent) const
	{
		Item* parentItem;

		if (!parent.isValid())
			parentItem = rootItem.get();
		else
			parentItem = static_cast<Item*>(parent.internalPointer());

		Item* childItem = parentItem->child(row);
		if (childItem)
			return createIndex(row, column, childItem);
		else
			return QModelIndex();
	}

	QModelIndex ShapeModel::parent(const QModelIndex &index) const
	{
		if (!index.isValid())
			return QModelIndex();

		Item *childItem = static_cast<Item*>(index.internalPointer());
		Item *parentItem = childItem->parent();

		if (parentItem == rootItem.get())
			return QModelIndex();

		return createIndex(parentItem->row(), 0, parentItem);
	}

	int ShapeModel::rowCount(const QModelIndex &parent) const
	{
		Item *parentItem;
		if (!parent.isValid())
			parentItem = rootItem.get();
		else
			parentItem = static_cast<Item*>(parent.internalPointer());

		return parentItem->childCount();
	}

	int ShapeModel::columnCount(const QModelIndex &parent) const
	{
		if (parent.isValid())
			return static_cast<Item*>(parent.internalPointer())->columnCount();
		else
			return rootItem->columnCount();
	}

	void ShapeModel::reload()
	{
		/*const QList<Shape>& shapeList = model->getShapes();
		Shape shape;
		foreach(shape, shapeList)
		{
			QList<QVariant> data;
			data << shape.getName();

			Item* item = new Item(data, rootItem.get());
			rootItem->appendChild(item);
		}

		reset();*/
	}

	void ShapeModel::shapeAdded(const Shape& shape)
	{
		/*removeRows(0, rowCount());
		rootItem->removeAll();
		reload();*/

		QList<QVariant> data;
		data << shape.getName();

		Item* item = new Item(data, rootItem.get());
		rootItem->appendChild(item);

		reset();
	}

	void ShapeModel::shapeRemoved(const Shape&)
	{
		removeRows(0, rowCount());
		rootItem->removeAll();
		reload();
	}

}
