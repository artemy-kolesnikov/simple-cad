#include "shape.h"

namespace Gui
{

	Shape::Shape(TopoDS_Shape& shape, QString name) :
		shape(shape), name(name)
	{
	}

	bool Shape::operator==(const Shape& other) const
	{
		return shape == other.getShape();
	}

	bool Shape::operator!=(const Shape& other) const
	{
		return shape != other.getShape();
	}

	QString Shape::getName() const
	{
		return name;
	}

	void Shape::setName(QString name)
	{
		this->name = name;
	}

	TopoDS_Shape Shape::getShape() const
	{
		return shape;
	}

}

