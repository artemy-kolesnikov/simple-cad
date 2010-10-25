#include "model.h"
#include "filehelper.h"

#include <stdlib.h>

#include <QDebug>

#include <Graphic3d_GraphicDevice.hxx>

static Handle(Graphic3d_GraphicDevice) defaultDevice;

Model::Model(QObject* parent) : QObject(parent)
{
	if(defaultDevice.IsNull())
		defaultDevice = new Graphic3d_GraphicDevice(getenv("DISPLAY"));

	TCollection_ExtendedString v3DName("Visu3D");

	viewer = new V3d_Viewer(defaultDevice, v3DName.ToExtString(), "", 1000.0, V3d_XposYnegZpos,
						Quantity_NOC_GRAY30, V3d_ZBUFFER, V3d_GOURAUD, V3d_WAIT,
						Standard_True, Standard_True, V3d_TEX_NONE);

	viewer->Init();
	viewer->SetDefaultLights();
	viewer->SetLightOn();

	context = new AIS_InteractiveContext(viewer);

	context->CurrentViewer()->CreateView();
}

void Model::loadModel(QString& fileName)
{
	this->fileName = fileName;

	if (!FileHelper::readFile(fileName, context))
	{
		qDebug() << "Read file" << fileName << "error";
		return;
	}

	Q_EMIT changed();
	Q_EMIT fileNameChanged(fileName);
}

void Model::saveModel(QString& fileName)
{
	if (!fileName.isEmpty())
	{
		this->fileName = fileName;
		Q_EMIT fileNameChanged(fileName);
	}
}

QString Model::getFileName() const
{
	return fileName;
}

Handle(AIS_InteractiveContext) Model::getContext() const
{
	return context;
}

