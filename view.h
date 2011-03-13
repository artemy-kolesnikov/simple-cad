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

#ifndef VIEW_HEADER
#define VIEW_HEADER

#include <QWidget>
#include <QPaintEvent>
#include <QPoint>

#include <V3d_View.hxx>

class Model;
class QRubberBand;

/**
 * Display loaded model
 */
class View : public QWidget
{
	Q_OBJECT

	enum CurrentAction
	{
		caNone,
		caMove,
		caRotate,
		caRectSelect
	};

	 enum ModifierKey
	 {
		 mkNone,
		 mkSpace
	 };

public:
	View(QWidget* parent = 0);
	~View();

	void setModel(Model* model);
	Model* getModel() const;

	QPaintEngine* paintEngine() const { return 0; }

Q_SIGNALS:
	void selectionChanged();

public Q_SLOTS:
	void viewFront();
	void viewBack();
	void viewTop();
	void viewBottom();
	void viewLeft();
	void viewRight();
	void viewDatumPlane();

protected:
	/*void paintEvent(QPaintEvent*);
	void resizeEvent(QResizeEvent*);
	void wheelEvent(QWheelEvent*);
	void mousePressEvent(QMouseEvent*);
	void mouseReleaseEvent(QMouseEvent*);
	void mouseMoveEvent(QMouseEvent*);
	void keyPressEvent(QKeyEvent*);
	void keyReleaseEvent(QKeyEvent*);*/

private:
	void createUI();
	void init();

	void onLButtonDown(const int flags, const QPoint point);
	void onRButtonDown(const int flags, const QPoint point);
	void onMButtonDown(const int flags, const QPoint point);

	void paintGl();

	static int paintCallBack(Aspect_Drawable drawable, void* userData,
		Aspect_GraphicCallbackStruct* data);

	void selectHook();

	Model* model;
	//Handle(V3d_View) view;
	bool firstPaint;

	QPoint pressedPoint;
	QPoint panPoint;
	QRubberBand* rectBand; 

	CurrentAction curAction;
	ModifierKey modKey;
};

#endif // VIEW_HEADER

