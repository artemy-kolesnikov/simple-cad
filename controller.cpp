#include "controller.h"

Controller::Controller(QObject* parent) : QObject(parent)
{
}

void Controller::updateView()
{
	Q_EMIT updateViewRequest();
}

void Controller::loadModel(QString& fileName)
{
	Q_EMIT loadModelRequest(fileName);
}

void Controller::saveModel(QString& fileName)
{
	Q_EMIT saveModelRequest(fileName);
}

