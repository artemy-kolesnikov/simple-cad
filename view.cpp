#include "view.h"
#include "model.h"

#include <QColormap>
#include <QWheelEvent>
#include <QDebug>

#include <Xw_Window.hxx>
#include <Graphic3d_GraphicDevice.hxx>

#include <QX11Info>
#include <GL/glx.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#include <X11/Xmu/StdCmap.h>
#include <X11/Xlib.h>
#include <Xw_Window.hxx>
#include <Graphic3d_GraphicDevice.hxx>

View::View(QWidget* parent) : QWidget(parent),
	model(0), firstPaint(true), pressedX(0), pressedY(0), curAction(caNone)
{
	createUI();
}

void View::createUI()
{
	XSynchronize(x11Info().display(), true);

	setAttribute(Qt::WA_PaintOnScreen);
	setAttribute(Qt::WA_NoSystemBackground);

	XVisualInfo* pVisualInfo;
	if (x11Info().display())
	{
		//Initialization with the default VisualID
		Visual *v = DefaultVisual(x11Info().display(), DefaultScreen(x11Info().display()));
		/*int visualID = */
		XVisualIDFromVisual(v);

		//Here we use the settings from Optimizer_ViewInfo::TxglCreateWindow()
		int visualAttr[] = { GLX_RGBA, GLX_DEPTH_SIZE, 1, GLX_RED_SIZE, 1, GLX_GREEN_SIZE, 1,
							GLX_BLUE_SIZE, 1, GLX_DOUBLEBUFFER, None };
		pVisualInfo = ::glXChooseVisual(x11Info().display(), DefaultScreen(x11Info().display()), visualAttr);

		if (isVisible())
			hide();

		XSetWindowAttributes a;

		Window p = RootWindow(x11Info().display(), DefaultScreen(x11Info().display()));
		a.colormap = XCreateColormap(x11Info().display(), RootWindow(x11Info().display(), pVisualInfo->screen),
			pVisualInfo->visual, AllocNone);
	
		QColor color = palette().color(backgroundRole());
		QColormap colmap = QColormap::instance();
		a.background_pixel = colmap.pixel(color);
		a.border_pixel = colmap.pixel(Qt::black);
		if (parentWidget())
			p = parentWidget()->winId();

		Window w = XCreateWindow(x11Info().display(), p,  x(), y(), width(), height(),
			0, pVisualInfo->depth, InputOutput,  pVisualInfo->visual,
			CWBackPixel | CWBorderPixel | CWColormap, &a);
		Window *cmw;
		Window *cmwret;
		int count;
		if (XGetWMColormapWindows(x11Info().display(), topLevelWidget()->winId(), &cmwret, &count))
		{
			cmw = new Window[count+1];
			memcpy((char *)cmw, (char *)cmwret, sizeof(Window)*count);
			XFree((char *)cmwret);
			int i;
			for (i = 0; i < count; ++i)
			{
				if (cmw[i] == winId())  // replace old window 
				{
					cmw[i] = w;
					break;
				}
			}
			if (i >= count)			 // append new window
				cmw[count++] = w;
		}
		else
		{
			count = 1;
			cmw = new Window[count];
			cmw[0] = w;
		}
		// Creating new window (with good VisualID) for this widget
		create(w);
		XSetWMColormapWindows(x11Info().display(), topLevelWidget()->winId(), cmw, count);
		delete [] cmw;

		if (isVisible())
			show();
		if (pVisualInfo)
			XFree((char *)pVisualInfo);
		XFlush(x11Info().display());
	}
	setMouseTracking(true);
}

void View::setModel(Model* model)
{
	this->model = model;

	connect(model, SIGNAL(changed()),
		this, SLOT(updateView()));
}

Model* View::getModel() const
{
	return model;
}

void View::updateView()
{
}

void View::init()
{
	view = model->getContext()->CurrentViewer()->CreateView();

	int windowHandle = (int)winId();
	short hi, lo;
	lo = (short) windowHandle;
	hi = (short) (windowHandle >> 16);
	Handle(Xw_Window) hWnd = new Xw_Window(Handle(Graphic3d_GraphicDevice)::
		DownCast(model->getContext()->CurrentViewer()->Device()),(int) hi,(int) lo,Xw_WQ_SAMEQUALITY);
	view->SetWindow(hWnd);
    if (!hWnd->IsMapped())
		hWnd->Map();
	view->SetBackgroundColor(Quantity_NOC_BLACK);
	view->MustBeResized();
}

void View::paintEvent(QPaintEvent*)
{
	if (firstPaint)
	{
		init();
		firstPaint = false;
	}

	view->Redraw();
}

void View::resizeEvent(QResizeEvent*)
{
	if( !view.IsNull() )
	{
		view->MustBeResized();
	}
}

void View::wheelEvent(QWheelEvent* event)
{
	if (event->delta() < 0)
		view->SetScale(1.05);
	else
		view->SetScale(0.95);

	view->SetViewMappingDefault();
}

void View::mousePressEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton)
		onLButtonDown((event->buttons() | event->modifiers()), event->pos());
	else if (event->button() == Qt::MidButton)
		onMButtonDown(event->buttons() | event->modifiers(), event->pos());
	else if (event->button() == Qt::RightButton)
		onRButtonDown(event->buttons() | event->modifiers(), event->pos() );
}

void View::mouseMoveEvent(QMouseEvent* event)
{
	QPoint point = event->pos();

	switch (curAction)
	{
		case caMove:
		{
			view->Pan(point.x() - pressedX, pressedY - point.y());
			break;
		}
		case caRotate:
		{
			view->Rotation(point.x(), point.y());
			view->Redraw();
			break;
		}
		default:
		{
			break;
		}
	}

	pressedX = point.x();
	pressedY = point.y();
}

void View::mouseReleaseEvent(QMouseEvent*)
{
	curAction = caNone;
}

void View::keyPressEvent(QKeyEvent* event)
{
	if (event->key() == Qt::Key_Space)
	{
		if (curAction == caNone)
			curAction = caRotate;
	}
}

void View::keyReleaseEvent(QKeyEvent* event)
{
	if (event->key() == Qt::Key_Space)
		curAction = caNone;
}

void View::onLButtonDown(const int, const QPoint point)
{
	pressedX = point.x();
	pressedY = point.y();

	if (curAction == caNone)
		curAction = caMove;
	else if (curAction == caRotate)
	{
		view->StartRotation(point.x(), point.y());
	}
}

void View::onRButtonDown(const int, const QPoint point)
{
	pressedX = point.x();
	pressedY = point.y();
}

void View::onMButtonDown(const int, const QPoint point)
{
	pressedX = point.x();
	pressedY = point.y();
}

