#include "createactionlist.h"
#include "primitivesettingswidget.h"
#include "widgetdialog.h"

#include <gp_Ax3.hxx>

#include <QString>

#include <boost/shared_ptr.hpp>

namespace Gui
{

	QString CreateActionList::getName() const
	{
		return QString::tr("Create");
	}

	void CreateActionList::createActions()
	{
		QAction* acBox = new QAction(tr("Box"), this);
		connect(acBox, SIGNAL(triggered()), this, SLOT(createBox()));
		actionList->push_back(acBox);

		QAction* acCylinder = new QAction(tr("Cylinder"), this);
		connect(acCylinder, SIGNAL(triggered()), this, SLOT(createCylinder()));
		actionList->push_back(acCylinder);

		QAction* acSphere = new QAction(tr("Sphere"), this);
		connect(acSphere, SIGNAL(triggered()), this, SLOT(createSphere()));
		actionList->push_back(acSphere);

		QAction* acCone = new QAction(tr("Cone"), this);
		connect(acCone, SIGNAL(triggered()), this, SLOT(createCone()));
		actionList->push_back(acCone);

		QAction* acTorus = new QAction(tr("Torus"), this);
		connect(acTorus, SIGNAL(triggered()), this, SLOT(createTorus()));
		actionList->push_back(acTorus);

		QAction* acPlane = new QAction(tr("Plane"), this);
		connect(acPlane, SIGNAL(triggered()), this, SLOT(createPlane()));
		actionList->push_back(acPlane);

		QAction* acEllipsoid = new QAction(tr("Ellipsoid"), this);
		connect(acEllipsoid, SIGNAL(triggered()), this, SLOT(createEllipsoid()));
		actionList->push_back(acEllipsoid);
	}

	void CreateActionList::createBox()
	{
		PrimitiveSettingsWidget* settingsWidget = new PrimitiveSettingsWidget(this);
		settingsWidget->setType(PrimitiveSettingsWidget::ptBox);
		WidgetDialog* dlg = new WidgetDialog(this);
		dlg->setCentralWidget(settingsWidget);
		if (dlg->exec() == QDialog::Accepted)
		{
			gp_Ax3 axis(settingsWidget->getPoint(), settingsWidget->getDir());
			float height = settingsWidget->getHeight();
			float width = settingsWidget->getWidth();
			float lenght = settingsWidget->getLength();
			//model->createBox(axis, height, width, lenght);
		}
	}

	void CreateActionList::createCylinder()
	{
		PrimitiveSettingsWidget* settingsWidget = new PrimitiveSettingsWidget(this);
		settingsWidget->setType(PrimitiveSettingsWidget::ptCylinder);
		WidgetDialog* dlg = new WidgetDialog(this);
		dlg->setCentralWidget(settingsWidget);
		if (dlg->exec() == QDialog::Accepted)
		{
			gp_Ax3 axis(settingsWidget->getPoint(), settingsWidget->getDir());
			float radius = settingsWidget->getRadius1();
			float height = settingsWidget->getHeight();
			float angle = settingsWidget->getAngle();
			//model->createCylinder(axis, radius, height, angle);
		}
	}

	void CreateActionList::createSphere()
	{
		PrimitiveSettingsWidget* settingsWidget = new PrimitiveSettingsWidget(this);
		settingsWidget->setType(PrimitiveSettingsWidget::ptSphere);
		WidgetDialog* dlg = new WidgetDialog(this);
		dlg->setCentralWidget(settingsWidget);
		if (dlg->exec() == QDialog::Accepted)
		{
			gp_Ax3 axis(settingsWidget->getPoint(), settingsWidget->getDir());
			float radius = settingsWidget->getRadius1();
			float angle = settingsWidget->getAngle();
			//model->createSphere(axis, radius, angle);
		}
	}

	void CreateActionList::createCone()
	{
		PrimitiveSettingsWidget* settingsWidget = new PrimitiveSettingsWidget(this);
		settingsWidget->setType(PrimitiveSettingsWidget::ptCone);
		WidgetDialog* dlg = new WidgetDialog(this);
		dlg->setCentralWidget(settingsWidget);
		if (dlg->exec() == QDialog::Accepted)
		{
			gp_Ax3 axis(settingsWidget->getPoint(), settingsWidget->getDir());
			float radius1 = settingsWidget->getRadius1();
			float radius2 = settingsWidget->getRadius2();
			float height = settingsWidget->getHeight();
			float angle = settingsWidget->getAngle();
			//model->createCone(axis, radius1, radius2, height, angle);
		}
	}

	void CreateActionList::createTorus()
	{
		PrimitiveSettingsWidget* settingsWidget = new PrimitiveSettingsWidget(this);
		settingsWidget->setType(PrimitiveSettingsWidget::ptTorus);
		WidgetDialog* dlg = new WidgetDialog(this);
		dlg->setCentralWidget(settingsWidget);
		if (dlg->exec() == QDialog::Accepted)
		{
			gp_Ax3 axis(settingsWidget->getPoint(), settingsWidget->getDir());
			float radius1 = settingsWidget->getRadius1();
			float radius2 = settingsWidget->getRadius2();
			float angle = settingsWidget->getAngle();
			//model->createTorus(axis, radius1, radius2, angle, 0, 0);
		}
	}

	void CreateActionList::createPlane()
	{
		PrimitiveSettingsWidget* settingsWidget = new PrimitiveSettingsWidget(this);
		settingsWidget->setType(PrimitiveSettingsWidget::ptPlane);
		WidgetDialog* dlg = new WidgetDialog(this);
		dlg->setCentralWidget(settingsWidget);
		if (dlg->exec() == QDialog::Accepted)
		{
			gp_Ax3 axis(settingsWidget->getPoint(), settingsWidget->getDir());
			float height = settingsWidget->getHeight();
			float width = settingsWidget->getWidth();
			//model->createPlane(axis, height, width);
		}
	}

	void CreateActionList::createEllipsoid()
	{
		PrimitiveSettingsWidget* settingsWidget = new PrimitiveSettingsWidget(this);
		settingsWidget->setType(PrimitiveSettingsWidget::ptPlane);
		WidgetDialog* dlg = new WidgetDialog(this);
		dlg->setCentralWidget(settingsWidget);
		if (dlg->exec() == QDialog::Accepted)
		{
			gp_Ax3 axis(settingsWidget->getPoint(), settingsWidget->getDir());
			float height = settingsWidget->getHeight();
			float width = settingsWidget->getWidth();
			//model->createPlane(axis, height, width);
		}
	}

}
