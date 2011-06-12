/*
 * Simple CAD System
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

#include "view.h"
#include "model.h"
#include "cadapplication.h"
#include "inventorviewer.h"
#include "interactiveview.h"
#include "viewershape.h"

#include <QColormap>
#include <QWheelEvent>
#include <QDebug>
#include <QList>
#include <QHBoxLayout>
#include <QFocusEvent>

#include <algorithm>

#include <BRep_Tool.hxx>
#include <Geom_Axis2Placement.hxx>
#include <Geom_Plane.hxx>
#include <Geom_Surface.hxx>
#include <TopExp_Explorer.hxx> 
#include <TopoDS.hxx>
#include <TopoDS_Face.hxx> 
#include <TopoDS_Shape.hxx>
#include <gp_Ax3.hxx>

#include <Inventor/nodes/SoGroup.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoSelection.h>
#include <Inventor/actions/SoSearchAction.h>

#include <commandmessage.h>
#include <shape.h>
#include "viewprovider.h"

namespace Gui
{

	View::View(QWidget* parent) : QWidget(parent),
		model(0), curAction(caNone),
		modKey(mkNone), selectedShape(0)
	{
		createUI();
	}

	View::~View()
	{
	}

	void View::createUI()
	{
		inventorViewer.reset(new InventorViewer(this));

		connect(inventorViewer->getEventObject(), SIGNAL(pathSelected(SoPath*)),
			this, SLOT(pathSelected(SoPath*)));
		connect(inventorViewer->getEventObject(), SIGNAL(pathDeselected(SoPath*)),
			this, SLOT(pathDeselected(SoPath*)));

		QHBoxLayout *layout = new QHBoxLayout(this);
		layout->setContentsMargins(QMargins(0, 0, 0, 0));;
		layout->addWidget(inventorViewer->getWidget());

		//setMouseTracking(true);

		viewProvider.reset(new ViewProvider(*inventorViewer.get()));
	}

	void View::setModel(Model* model)
	{
		this->model = model;

		connect(model, SIGNAL(shapeAdded(const Shape&)),
			this, SLOT(shapeAdded(const Shape&)));
		connect(model, SIGNAL(shapeRemoved(const Shape&)),
			this, SLOT(shapeRemoved(const Shape&)));
	}

	Model* View::getModel() const
	{
		return model;
	}

	void View::setInteractiveView(InteractiveView* view)
	{
		interactiveView.reset(view);
		inventorViewer->setInteractiveView(interactiveView.get());
	}

	void View::receive(Common::Message* msg)
	{
		Sketcher::CommandMessage* message = dynamic_cast<Sketcher::CommandMessage*>(msg);
		if (message && interactiveView.get())
			interactiveView->receive(message);
	}

	const ViewerShape* View::getSelectedShape() const
	{
		return selectedShape;
	}

	void View::viewFront()
	{
		float root = (float)(sqrt(2.0)/2.0);
		inventorViewer->setCameraOrientation(SbRotation(-root, 0, 0, -root));
		inventorViewer->viewAll();
	}

	void View::viewBack()
	{
		float root = (float)(sqrt(2.0)/2.0);
		inventorViewer->setCameraOrientation(SbRotation(0, root, root, 0));
		inventorViewer->viewAll();
	}

	void View::viewTop()
	{
		inventorViewer->setCameraOrientation(SbRotation(0, 0, 0, 1));
		inventorViewer->viewAll();
	}

	void View::viewBottom()
	{
		inventorViewer->setCameraOrientation(SbRotation(-1, 0, 0, 0));
		inventorViewer->viewAll();
	}

	void View::viewLeft()
	{
		inventorViewer->setCameraOrientation(SbRotation(-0.5, 0, 0, -0.5));
		inventorViewer->viewAll();
	}

	void View::viewRight()
	{
		inventorViewer->setCameraOrientation(SbRotation(0.5, 0, 0, 0.5));
		inventorViewer->viewAll();
	}

	void View::viewAll()
	{
		inventorViewer->viewAll();
	}

	void View::viewAxometric()
	{
        inventorViewer->setCameraOrientation(SbRotation
            (-0.353553f, -0.146447f, -0.353553f, -0.853553f));
		inventorViewer->viewAll();
	}

	void View::viewDatumPlane()
	{
		//view->SetFront();
	}

	void View::shapeAdded(const Shape& shape)
	{
		/*SoGroup* faces = new SoGroup();
		viewProvider.computeFaces(faces, shape, 1);
		SoSelection* root = inventorViewer->getRootNode();
		root->addChild(faces);*/
		//inventorViewer->setSceneGraph(faces);
		viewProvider->display(new ViewerShape("", shape));
		inventorViewer->viewAll();
	}

	void View::shapeRemoved(const Shape& shape)
	{
		const ViewerShape* viewerShape = 0;
		viewProvider->getViewerShape(shape, viewerShape);

		assert(viewerShape);

		viewProvider->remove(viewerShape);
	}

	void View::pathSelected(SoPath* path)
	{
		int pathLength = path->getLength();
		assert(pathLength > 2);

		SoNode* node = path->getNodeFromTail(pathLength - 2);

        if (SoGroup::getClassTypeId() == node->getTypeId())
		{
			/*SoSearchAction sa;
			sa.setNode(node);
			sa.setInterest(SoSearchAction::FIRST);
			sa.apply(inventorViewer->getRootNode());
			SoPath* nodePath = sa.getPath();*/

			viewProvider->getViewerShape(static_cast<SoGroup*>(node), selectedShape);

			assert(selectedShape);

			Q_EMIT selectionChanged();
		}
	}

	void View::pathDeselected(SoPath* path)
	{
	}

	/*void View::selectHook()
	{
		boost::shared_ptr<AIS_SequenceOfInteractive> shapes = model->getSelectedShapes();
		if (shapes->Length() == 1)
		{
			TopoDS_Shape shape = Handle(AIS_Shape)::DownCast(shapes->Value(1))->Shape();

			for(TopExp_Explorer faceExplorer(shape, TopAbs_FACE); faceExplorer.More(); faceExplorer.Next())
			{
				TopoDS_Face face = TopoDS::Face(faceExplorer.Current());

				Handle(Geom_Surface) surface = BRep_Tool::Surface(face);

				if(surface->DynamicType() == STANDARD_TYPE(Geom_Plane))
				{
					Handle(Geom_Plane) plane = Handle(Geom_Plane)::DownCast(surface);
					gp_Ax3 pos = plane->Position();
					gp_Dir dir = pos.Direction();
					gp_Vec vec(dir);

					//gp_Vec v(gp_Pnt(2, 34, 54), gp_Pnt(45, 65, 22));
					gp_Vec v(gp_Pnt(0, 0, 0), gp_Pnt(1, 0, 0));
					gp_Dir d(v);

					model->getContext()->CurrentViewer()->SetPrivilegedPlane(pos);
				}
			}
		}
	}*/

}

