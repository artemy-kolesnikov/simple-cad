#ifndef CHILD_WINDOW_HEADER
#define CHILD_WINDOW_HEADER

#include <QMdiSubWindow>
#include <QString>

class Model;
class View;
class Controller;

class ChildWindow : public QMdiSubWindow
{
	Q_OBJECT

public:
	ChildWindow(Model* model, QWidget* parent = 0);

	Controller* getController();

protected:
	void keyPressEvent(QKeyEvent*);
	void keyReleaseEvent(QKeyEvent*);

private Q_SLOTS:
	void modelChanged();
	void modelFileNameChanged(QString& fileName);

private:
	void createUI();

	Model* model;
	View* view;
	Controller* controller;
};

#endif // CHILD_WINDOW_HEADER
