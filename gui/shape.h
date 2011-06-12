#ifndef SHAPE_HEADER
#define SHAPE_HEADER

#include <TopoDS_Shape.hxx>
#include <QString>

namespace Gui
{

	class Shape
	{
	public:
		Shape() {}
		Shape(TopoDS_Shape& shape, QString name = QString());

		bool operator==(const Shape& other) const;
		bool operator!=(const Shape& other) const;

		QString getName() const;
		void setName(QString name);

		TopoDS_Shape getShape() const;

	private:
		TopoDS_Shape shape;
		QString name;
	};

}

#endif // SHAPE_HEADER
