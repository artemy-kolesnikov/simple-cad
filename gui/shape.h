#ifndef SHAPE_HEADER
#define SHAPE_HEADER

#include <TopoDS_Shape.hxx>
#include <gp_Trsf.hxx>
#include <QString>

namespace Gui
{

	class Shape
	{
		friend class Model;
	public:
		Shape(TopoDS_Shape shape, QString name = QString());
		Shape(const Shape& other);
		~Shape();

		bool operator==(const Shape& other) const;
		bool operator!=(const Shape& other) const;

		QString getName() const;
		void setName(QString name);

		TopoDS_Shape getShape() const;

		gp_Trsf getTransform() const;
		void setTransform(const gp_Trsf& value);

	private:
		TopoDS_Shape shape;
		QString name;
	};

}

#endif // SHAPE_HEADER
