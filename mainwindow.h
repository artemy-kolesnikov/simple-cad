#ifndef MAIN_WINDOW_HEADER
#define MAIN_WINDOW_HEADER

#include <QMainWindow>
#include <QList>
#include <QMap>

class QMdiArea;
class QDockWidget;
class QMenuBar;
class QAction;
class QTreeView;
class Model;

class ChildWindow;

class MainWindow : public QMainWindow
{
	Q_OBJECT
public:
	MainWindow(QWidget* parent = 0);

private Q_SLOTS:
	void elementsVisChanged(bool);
	void about();
	void openModel();
	ChildWindow* newChildWindow();

private:
	void createUI();
	void createDockWidget();
	void createMenuAndActions();

	QMdiArea* mdiArea;
	QDockWidget* elementsDock;
	QMenuBar* mainMenuBar;
	QTreeView* elementsTreeView;

	QAction* acExit;
	QAction* acAbout;
	QAction* acOpenModel;
	QAction* acNewModel;

	QMap<QWidget*, Model*> windowModelMap;
};

#endif // MAIN_WINDOW_HEADER
