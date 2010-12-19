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
		caRotate
	};

	 enum ModifierKey
	 {
		 mkNone,
		 mkSpace
	 };

public:
	View(QWidget* parent = 0);

	void setModel(Model* model);
	Model* getModel() const;

Q_SIGNALS:
	void selectionChanged();

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
	ModifierKey modKey;
};

#endif // VIEW_HEADER

