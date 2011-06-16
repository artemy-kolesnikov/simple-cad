/*
 * Simple CAD System
 * Based on opencascade (www.opencascade.org)
 *
 * Copyright (C) 2010 Artemy Kolesnikov <artemy.kolesnikov@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include "propertieswidget.h"

#include "model.h"

#include <QGridLayout>
#include <QVBoxLayout>
#include <QComboBox>
#include <QStringList>
#include <QLabel>
#include <QCheckBox>

#include <viewershape.h>

namespace Gui
{

	PropertiesWidget::PropertiesWidget(QWidget* parent) : QWidget(parent)
	{
		createUI();
	}

	void PropertiesWidget::setModel(Model* model)
	{
		this->model = model;
	}

	void PropertiesWidget::setShape(ViewerShape* shape)
	{
		this->shape = shape;

		updateView();
	}

	void PropertiesWidget::createUI()
	{
		QVBoxLayout* vLayout = new QVBoxLayout(this);
		setLayout(vLayout);

		int row = 0;

		QGridLayout* gridLayout = new QGridLayout();
		vLayout->addLayout(gridLayout);

		QLabel* lbMaterial = new QLabel(tr("Material:"), this);
		gridLayout->addWidget(lbMaterial, row, 0);

		cbMaterial = new QComboBox(this);
		cbMaterial->setEditable(false);
		//cbMaterial->addItems(Model::getMaterials());

		connect(cbMaterial, SIGNAL(activated(int)),
			this, SLOT(materialChanged(int)));

		gridLayout->addWidget(cbMaterial, row, 1);

		++row;

		/*QLabel* lbShadow = new QLabel(tr("Shaded"), this);
		gridLayout->addWidget(lbShadow, row, 0);

		cbShaded = new QCheckBox(this);
		gridLayout->addWidget(cbShaded, row, 1);

		connect(cbShaded, SIGNAL(stateChanged(int)),
			this, SLOT(shaddedChanged(int)));*/

		++row;

		vLayout->addStretch();
	}

	void PropertiesWidget::updateView()
	{
	}

}

