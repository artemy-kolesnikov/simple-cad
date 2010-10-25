#ifndef CONTROOLER_HEADER
#define CONTROOLER_HEADER

#include <QObject>

class Controller : public QObject
{
	Q_OBJECT

public:
	Controller(QObject* parent = 0);

public Q_SLOTS:
	void updateView();
	void loadModel(QString& fileName);
	void saveModel(QString& fileName);

Q_SIGNALS:
	void updateViewRequest();
	void loadModelRequest(QString& fileName);
	void saveModelRequest(QString& fileName);
};

#endif // CONTROOLER_HEADER
