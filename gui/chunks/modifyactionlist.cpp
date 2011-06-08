#include "modifyactionlist.h"

#include <QString>

namespace Gui
{

	QString ModifyActionList::getName() const
	{
		return QString::tr("Operations");
	}

	void ModifyActionList::createActions()
	{
		/*QAction* acStamp = new QAction(tr("Выдавливание"), this);
		connect(acStamp, SIGNAL(triggered()), this, SLOT(makePrism()));
		actionList->push_back(acStamp);

		QAction* acRotation = new QAction(tr("Вращение"), this);
		actionList->push_back(acRotation);

		QAction* acAddition = new QAction(tr("Сложение"), this);
		actionList->push_back(acAddition);

		QAction* acSubstract = new QAction(tr("Вычитание"), this);
		actionList->push_back(acSubstract);*/

		QAction* acFusion = new QAction(tr("Fuse"), this);
		connect(acFusion, SIGNAL(triggered()), this, SLOT(shapesFusion()));
		actionList->push_back(acFusion);

		QAction* acCommon = new QAction(tr("Common"), this);
		connect(acCommon, SIGNAL(triggered()), this, SLOT(shapesCommon()));
		actionList->push_back(acCommon);

		QAction* acCut = new QAction(tr("Cut"), this);
		connect(acCut, SIGNAL(triggered()), this, SLOT(shapesCut()));
		actionList->push_back(acCut);

		QAction* acRemoveShape = new QAction(tr("Remove"), this);
		acRemoveShape->setShortcut(Qt::Key_Delete);
		connect(acRemoveShape, SIGNAL(triggered()), this, SLOT(removeShape()));
		actionList->push_back(acRemoveShape);
	}

	void CreateActionList::removeShape()
	{
		/*QString msgText("Вы действительно хотите удалить %1");

		boost::shared_ptr<AIS_SequenceOfInteractive> shapes = model->getCurrentShapes();
		if (shapes->Length() == 1)
			msgText = msgText.arg(tr("объект"));
		else
			msgText = msgText.arg(tr("выбранные объекты"));

		if (QMessageBox::question(this, tr("Удаление объекта"),
			msgText,
			QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel) == QMessageBox::Yes)
		{
			for (int i = 1; i <= shapes->Length(); ++i)
			{
				model->removeShape(shapes->Value(i));
			}
		}*/
	}


	void CreateActionList::shapesFusion()
	{
		/*boost::shared_ptr<AIS_SequenceOfInteractive> shapes = model->getCurrentShapes();
		if (shapes->Length() == 2)
		{
			Handle(AIS_Shape) shape1 = Handle(AIS_Shape)::DownCast(shapes->Value(1));
			Handle(AIS_Shape) shape2 = Handle(AIS_Shape)::DownCast(shapes->Value(2));

			model->fuse(shape1, shape2);
		}
		else
			QMessageBox::critical(this, tr("Ошибка"), tr("Выберите два объекта"));*/
	}

	void CreateActionList::shapesCommon()
	{
		/*boost::shared_ptr<AIS_SequenceOfInteractive> shapes = model->getCurrentShapes();
		if (shapes->Length() == 2)
		{
			Handle(AIS_Shape) shape1 = Handle(AIS_Shape)::DownCast(shapes->Value(1));
			Handle(AIS_Shape) shape2 = Handle(AIS_Shape)::DownCast(shapes->Value(2));

			model->common(shape1, shape2);
		}
		else
			QMessageBox::critical(this, tr("Ошибка"), tr("Выберите два объекта"));*/
	}

	void CreateActionList::shapesCut()
	{
		/*boost::shared_ptr<AIS_SequenceOfInteractive> shapes = model->getCurrentShapes();
		if (shapes->Length() == 2)
		{
			Handle(AIS_Shape) shape1 = Handle(AIS_Shape)::DownCast(shapes->Value(1));
			Handle(AIS_Shape) shape2 = Handle(AIS_Shape)::DownCast(shapes->Value(2));

			model->cut(shape1, shape2);
		}
		else
			QMessageBox::critical(this, tr("Ошибка"), tr("Выберите два объекта"));*/
	}

}
