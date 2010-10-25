#ifndef MODEL_HEADER
#define MODEL_HEADER

#include <QObject>

#include <AIS_InteractiveContext.hxx>
#include <V3d_Viewer.hxx>

class Model : public QObject
{
	Q_OBJECT

public:
	Model(QObject* parent = 0);

	QString getFileName() const;

	Handle(AIS_InteractiveContext) getContext() const;

Q_SIGNALS:
	void changed();
	void fileNameChanged(QString& newFileName);

public Q_SLOTS:
	void loadModel(QString& fileName);
	void saveModel(QString& fileName);

private:
	QString fileName;

	Handle(V3d_Viewer) viewer;
	Handle(AIS_InteractiveContext) context;
};

#endif // MODEL_HEADER
