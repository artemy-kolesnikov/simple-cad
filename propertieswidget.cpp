#include "propertieswidget.h"

#include "model.h"

#include <QGridLayout>
#include <QVBoxLayout>
#include <QComboBox>
#include <QStringList>
#include <QLabel>
#include <QCheckBox>

PropertiesWidget::PropertiesWidget(QWidget* parent) : QWidget(parent)
{
	createUI();
}

void PropertiesWidget::setModel(Model* model)
{
	this->model = model;

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
	cbMaterial->addItems(Model::getMaterials());

	connect(cbMaterial, SIGNAL(activated(int)),
		this, SLOT(materialChanged(int)));

	gridLayout->addWidget(cbMaterial, row, 1);

	++row;

	QLabel* lbShadow = new QLabel(tr("Shaded"), this);
	gridLayout->addWidget(lbShadow, row, 0);

	cbShaded = new QCheckBox(this);
	gridLayout->addWidget(cbShaded, row, 1);

	connect(cbShaded, SIGNAL(stateChanged(int)),
		this, SLOT(shaddedChanged(int)));

	++row;

	vLayout->addStretch();
}

void PropertiesWidget::updateView()
{
}

void PropertiesWidget::materialChanged(int value)
{
	Q_EMIT materialChanged(static_cast<Graphic3d_NameOfMaterial>(value));
}

void PropertiesWidget::shaddedChanged(int value)
{
	bool state = (value == Qt::Checked);
	Q_EMIT shadedChanged(state);
}

