#include "shape.h"

#include <QDebug>
#include <cassert>

namespace Gui
{

	Shape::Shape(TopoDS_Shape shape, QString name) :
		shape(shape), name(name)
	{
		assert(!shape.IsNull());
	}

	Shape::~Shape()
	{
	}

	Shape::Shape(const Shape& other)
	{
		assert(!other.shape.IsNull());
		name = other.name;
		shape = other.shape;
	}

	bool Shape::operator==(const Shape& other) const
	{
		//TODO: Поменять способ идентификации, т.к. не известно, меняется или указатель со временем
		return shape.TShape().Access() == other.shape.TShape().Access();
	}

	bool Shape::operator!=(const Shape& other) const
	{
		return !(*this == other);
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
		assert(!shape.IsNull());
		return shape;
	}

	gp_Trsf Shape::getTransform() const
	{
		return shape.Location().Transformation();
	}

	void Shape::setTransform(const gp_Trsf& value)
	{
		shape.Location(TopLoc_Location(value));
	}

}

