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

#include "sketcherview.h"

#include <Inventor/Qt/viewers/SoQtViewer.h>
#include <Inventor/SbVec2f.h>
#include <Inventor/SbVec3f.h>
#include <Inventor/SoPickedPoint.h>
#include <Inventor/nodes/SoLineSet.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoVertexProperty.h>
#include <Inventor/nodes/SoCamera.h>
#include <Inventor/nodes/SoLight.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/projectors/SbPlaneProjector.h>
#include <Inventor/manips/SoTabBoxManip.h>

#include <commandmessage.h>
#include <rectangle.h>
#include <polyline.h>
#include <circle.h>
#include <sketchcommon.h>
namespace
{
	SbBool isTransformable(SoNode *myNode)
	{
	  if (myNode->isOfType(SoGroup::getClassTypeId())
		|| myNode->isOfType(SoShape::getClassTypeId())
		|| myNode->isOfType(SoCamera::getClassTypeId())
		|| myNode->isOfType(SoLight::getClassTypeId()))
		return TRUE;
	  else 
		return FALSE;
	}

	SoPath* createTransformPath(SoPath *inputPath)
	{
	  int pathLength = inputPath->getLength();
	  if (pathLength < 2) // Won't be able to get parent of tail
		return NULL;
	  
	  SoNode *tail = inputPath->getTail();
	  
	  // CASE 1: The tail is a node kit.
	  // Nodekits have built in policy for creating parts.
	  // The kit copies inputPath, then extends it past the 
	  // kit all the way down to the transform. It creates the
	  // transform if necessary.
	  if (tail->isOfType(SoBaseKit::getClassTypeId()))
	  {
		SoBaseKit *kit = (SoBaseKit *) tail;
		return kit->createPathToPart("transform",TRUE,inputPath);
	  }
	  
	  SoTransform *editXf = NULL;
	  SoGroup *parent;
	  
	  // CASE 2: The tail is not a group.
	  SbBool isTailGroup;
	  isTailGroup = tail->isOfType(SoGroup::getClassTypeId());
	  if (!isTailGroup)
	  {
		// 'parent' is node above tail. Search under parent right
		// to left for a transform. If we find a 'movable' node
		// insert a transform just left of tail.  
		parent = (SoGroup *) inputPath->getNode(pathLength - 2);
		int tailIndx = parent->findChild(tail);
		
		for (int i = tailIndx; (i >= 0) && (editXf == NULL);i--)
		{
		  SoNode *myNode = parent->getChild(i);
		  if (myNode->isOfType(SoTransform::getClassTypeId()))
			editXf = (SoTransform *) myNode;
		  else if (i != tailIndx && (isTransformable(myNode)))
			break;
		}

		if (editXf == NULL)
		{
		  editXf = new SoTransform;
		  parent->insertChild(editXf, tailIndx);
		}
	  }
	  // CASE 3: The tail is a group.
	  else
	  {
		// Search the children from left to right for transform 
		// nodes. Stop the search if we come to a movable node.
		// and insert a transform before it.
		int i;
		
		parent = (SoGroup *) tail;
		for (i = 0; (i < parent->getNumChildren()) && (editXf == NULL);	i++)
		{
		  SoNode *myNode = parent->getChild(i);
		  if (myNode->isOfType(SoTransform::getClassTypeId()))
			editXf = (SoTransform *) myNode;
		  else if (isTransformable(myNode))
			break;
		}

		if (editXf == NULL)
		{
		  editXf = new SoTransform;
		  parent->insertChild(editXf, i);
		}
	  }
	  
	  // Create 'pathToXform.' Copy inputPath, then make last
	  // node be editXf.
	  SoPath *pathToXform = NULL;
	  pathToXform = inputPath->copy();
	  pathToXform->ref();
	  if (!isTailGroup) // pop off the last entry.
		pathToXform->pop();
	  // add editXf to the end
	  int xfIndex   = parent->findChild(editXf);
	  pathToXform->append(xfIndex);
	  pathToXform->unrefNoDelete();
	  
	  return pathToXform;
	}

}

namespace Sketcher
{

	SketcherView::SketcherView(SbPlane plane, QObject* parent) :
		InteractiveView(parent), sketchPlane(plane) 
	{
		currentPrimitive = 0;

		planeProjector.reset(new SbPlaneProjector());

		drawGrid();

		boxManip = new SoTabBoxManip();
		boxManip->ref();
	}

	SketcherView::~SketcherView()
	{
		boxManip->unref();
	}

	SbVec3f SketcherView::projectPoint(SbVec2f& curPos)
	{
		assert(viewer);
		SbViewVolume volume(viewer->getCamera()->getViewVolume());
		return projectPoint(curPos, volume);
	}

	SbVec3f SketcherView::projectPoint(SbVec2f& curPos, const SbViewVolume& volume)
	{
		planeProjector->setViewVolume(volume);

		//SbPlane plane(curNormal, 0);
		//planeProjector->setPlane(plane);

		return planeProjector->project(curPos);
	}

	void SketcherView::receive(Common::Message* msg)
	{
		CommandMessage* message = dynamic_cast<CommandMessage*>(msg);
		assert(message);

		processMessage(message);
	}

	bool SketcherView::keyPressed(int key)
	{
		return false;
	}

	bool SketcherView::mouseMove(const SbVec2f& pos, const SbVec3f& norm,
		const SoPickedPoint* pp)
	{
		return false;
	}

	bool SketcherView::mouseButtonPressed(int Button, bool pressed, const SbVec2f &pos,
		const SbVec3f& norm, const SoPickedPoint* pp)
	{
		return false;
	}

	void SketcherView::pathSelected(SoPath* path)
	{
		xformPath = createTransformPath(path);
		if (xformPath)
		{
			xformPath->ref();
			boxManip->replaceNode(xformPath);
		}
	}

	void SketcherView::pathDeselected(SoPath*)
	{
		boxManip->replaceManip(xformPath, 0);
		xformPath->unref();
	}

	void SketcherView::drawGrid()
	{
		SoSeparator* gridSeparator = new SoSeparator();
		unselectableSeparator->addChild(gridSeparator);

		SoMaterial* gridMaterial = new SoMaterial();
		gridMaterial->diffuseColor.setValue(.33, .33, .33);
		gridSeparator->addChild(gridMaterial);

		SoLineSet* gridLineSet = new SoLineSet();

		SoVertexProperty* gridVertexProp = new SoVertexProperty();
		gridLineSet->vertexProperty = gridVertexProp;

		int vi = 0;
		const int lines = 10;
		const int step = 80;
		int l = 0;
		for (int i=0; i<=lines * step; i+=step)
		{
			gridVertexProp->vertex.set1Value(vi++, i, 0, -1);
			gridVertexProp->vertex.set1Value(vi++, i, lines * step, -1);
			gridLineSet->numVertices.set1Value(l++, 2);
		}

		for (int i=0; i<=lines * step; i+=step)
		{
			gridVertexProp->vertex.set1Value(vi++, 0, i, -1);
			gridVertexProp->vertex.set1Value(vi++, lines * step, i, -1);
			gridLineSet->numVertices.set1Value(l++, 2);
		}

		gridSeparator->addChild(gridLineSet);
		gridSeparator->addChild(gridVertexProp);
	}

	void SketcherView::processMessage(CommandMessage* msg)
	{
		createPrimitive(msg->getType());
	}

	void SketcherView::createPrimitive(PrimitiveType type)
	{
		// Если запрос на создание примитива поступил до
		// завершения конструирования предыдущего примитива
		if (currentPrimitive)
			resetCurrentPrimitive(true);

		if (ptRectangle == type)
		{
			Rectangle* r = new Rectangle(*this);
			r->ref();
			currentPrimitive = r;
		}
		else if (ptPolyline == type)
		{
			Polyline* pl = new Polyline(*this);
			pl->ref();
			currentPrimitive = pl;
		}
		else if (ptCircle == type)
		{
			Circle* c = new Circle(*this);
			c->ref();
			currentPrimitive = c;
		}

		currentPrimitive->handleEvents(true);
		selectableSeparator->addChild(currentPrimitive);

		connect(currentPrimitive->getEvents(), SIGNAL(created(Primitive2D*)),
			this, SLOT(primitiveCreated(Primitive2D*)));
		connect(currentPrimitive->getEvents(), SIGNAL(creationCanceled(Primitive2D*)),
			this, SLOT(primitiveCreationCanceled(Primitive2D*)));

	}

	void SketcherView::resetCurrentPrimitive(bool removeFromScene)
	{
		currentPrimitive->handleEvents(false);
		disconnect(currentPrimitive->getEvents(), SIGNAL(created(Primitive2D*)),
			this, SLOT(primitiveCreated(Primitive2D*)));
		disconnect(currentPrimitive->getEvents(), SIGNAL(creationCanceled(Primitive2D*)),
			this, SLOT(primitiveCreationCanceled(Primitive2D*)));

		if (removeFromScene)
			selectableSeparator->removeChild(currentPrimitive);

		currentPrimitive = 0;
	}

	void SketcherView::primitiveCreated(Primitive2D* primitive)
	{
		resetCurrentPrimitive(false);
	}

	void SketcherView::primitiveCreationCanceled(Primitive2D* primitive)
	{
		resetCurrentPrimitive(false);
	}

}

