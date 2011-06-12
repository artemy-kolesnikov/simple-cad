#ifndef SHAPE_MODEL_HEADER
#define SHAPE_MODEL_HEADER

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>
#include <memory>

namespace Gui
{

	class Model;
	class Shape;

	namespace _Impl
	{
		class Item;
	}

	class ShapeModel : public QAbstractItemModel
	{
		Q_OBJECT
	public:
		ShapeModel(Model* model);
		~ShapeModel();

		QVariant data(const QModelIndex &index, int role) const;
		Qt::ItemFlags flags(const QModelIndex &index) const;
		QVariant headerData(int section, Qt::Orientation orientation,
						int role = Qt::DisplayRole) const;
		QModelIndex index(int row, int column,
						const QModelIndex &parent = QModelIndex()) const;
		QModelIndex parent(const QModelIndex &index) const;
		int rowCount(const QModelIndex &parent = QModelIndex()) const;
		int columnCount(const QModelIndex &parent = QModelIndex()) const;

	private:
		void reload();

	private Q_SLOTS:
		void shapeAdded(const Shape& shape);
		void shapeRemoved(const Shape& shape);

	private:
		std::auto_ptr<_Impl::Item> rootItem;
		Model* model;
	};

}

#endif // SHAPE_MODEL_HEADER

