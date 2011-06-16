#include "shape.h"

#include <QDebug>
#include <cassert>
#include <BRepBuilderAPI_Transform.hxx>

#include <gp_Ax1.hxx>

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
		/*gp_Trsf shapeTrsfm;
		shapeTrsfm.SetRotation(gp_Ax1(gp_Pnt(0.,0.,0.),gp_Vec(0.,0.,1.)),2.*PI/5.);
		BRepBuilderAPI_Transform theTrsf(shape, T);
		shape = theTrsf.Shape();*/

		BRepBuilderAPI_Transform makeTrsf(value);
		makeTrsf.Perform(shape, false);

		TopoDS_Shape shape1 = makeTrsf.Shape();

		gp_Trsf shapeTrsfm = shape1.Location().Transformation();

		qDebug() << shapeTrsfm.Value(1, 1) << shapeTrsfm.Value(1, 2) << shapeTrsfm.Value(1, 3) << shapeTrsfm.Value(1, 4);
		qDebug() << shapeTrsfm.Value(2, 1) << shapeTrsfm.Value(2, 2) << shapeTrsfm.Value(2, 3) << shapeTrsfm.Value(2, 4);
		qDebug() << shapeTrsfm.Value(3, 1) << shapeTrsfm.Value(3, 2) << shapeTrsfm.Value(3, 3) << shapeTrsfm.Value(3, 4);
		qDebug() << "";

		//shape.Location(TopLoc_Location(value));
	}

}

