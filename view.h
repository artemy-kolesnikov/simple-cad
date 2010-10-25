#ifndef VIEW_HEADER
#define VIEW_HEADER

#include <QWidget>
#include <QPaintEvent>
#include <QPoint>

#include <V3d_View.hxx>

class Model;

class View : public QWidget
{
	Q_OBJECT

	enum CurrentAction
	{
		caNone = 0,
		caMove,
		caRotate
	};

public:
	View(QWidget* parent = 0);

	void setModel(Model* model);
	Model* getModel() const;

public Q_SLOTS:
	void updateView();

protected:
	void paintEvent(QPaintEvent*);
	void resizeEvent(QResizeEvent*);
	void wheelEvent(QWheelEvent*);
	void mousePressEvent(QMouseEvent*);
	void mouseReleaseEvent(QMouseEvent*);
	void mouseMoveEvent(QMouseEvent*);
	void keyPressEvent(QKeyEvent*);
	void keyReleaseEvent(QKeyEvent*);

private:
	void createUI();
	void init();

	void onLButtonDown(const int nFlags, const QPoint point);
	void onRButtonDown(const int nFlags, const QPoint point);
	void onMButtonDown(const int nFlags, const QPoint point);

	Model* model;
	Handle(V3d_View) view;
	bool firstPaint;

	int pressedX;
	int pressedY;

	CurrentAction curAction;
};

#endif // VIEW_HEADER

