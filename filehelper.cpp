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

#include "filehelper.h"

#include <QFileInfo>
#include <QString>
#include <QDebug>

#include <TopTools_HSequenceOfShape.hxx>
#include <TopoDS_Shape.hxx>
#include <BRep_Builder.hxx>
#include <IGESControl_Reader.hxx>
#include <BRepTools.hxx>
#include <STEPControl_Reader.hxx>
#include <Storage_Root.hxx>
#include <PTopoDS_HShape.hxx>
#include <FSD_File.hxx>
#include <ShapeSchema.hxx>
#include <Storage_HSeqOfRoot.hxx>
#include <PTColStd_PersistentTransientMap.hxx>
#include <MgtBRep.hxx>
#include <IGESControl_Writer.hxx>
#include <STEPControl_Writer.hxx>
#include <StlAPI_Writer.hxx>
#include <VrmlAPI_Writer.hxx>
#include <IGESControl_Controller.hxx>
#include <Interface_Static.hxx>
#include <AIS_Shape.hxx>

namespace
{

	enum FileType
	{
		ftUnknown = -1,
		ftBREP,
		ftCSFDB,
		ftIGES,
		ftSTEP,
		ftSTL,
		ftVRML
	};

	FileType getFileType(QString& fileName);
	bool importModel(FileType ft, QString& fileName, Handle(TopTools_HSequenceOfShape)& shapes);
	bool exportModel(FileType ft, QString& fileName, const Handle(AIS_InteractiveContext)& context);

	bool importBREP(QString& fileName, Handle(TopTools_HSequenceOfShape)& shapes);
	bool importIGES(QString& fileName, Handle(TopTools_HSequenceOfShape)& shapes);
	bool importSTEP(QString& fileName, Handle(TopTools_HSequenceOfShape)& shapes);
	bool importCSFDB(QString& fileName, Handle(TopTools_HSequenceOfShape)& shapes);

	bool exportBREP(QString& fileName, Handle(TopTools_HSequenceOfShape)& shapes);
	bool exportIGES(QString& fileName, Handle(TopTools_HSequenceOfShape)& shapes);
	bool exportSTEP(QString& fileName, Handle(TopTools_HSequenceOfShape)& shapes);
	bool exportCSFDB(QString& fileName, Handle(TopTools_HSequenceOfShape)& shapes);
	bool exportSTL(QString& fileName, Handle(TopTools_HSequenceOfShape)& shapes);
	bool exportVRML(QString& fileName, Handle(TopTools_HSequenceOfShape)& shapes);
	
	Handle(TopTools_HSequenceOfShape) getShapes(const Handle(AIS_InteractiveContext)& context);
}

void FileHelper::readFile(QString& fileName,
	Handle(TopTools_HSequenceOfShape)& shapes) throw(FileError)
{
	FileType ft = getFileType(fileName);

    if (!importModel(ft, fileName, shapes))
		throw FileError(QObject::tr("Ошибка импорта элементов"));

	/*if (shapes.IsNull() || !shapes->Length())
		throw FileError(QObject::tr("Ошибка чтения элементов"));

	for (int i = 1; i <= shapes->Length(); ++i)
		context->Display(new AIS_Shape(shapes->Value(i)), false);

	context->UpdateCurrentViewer();*/
}

void FileHelper::writeFile(QString& fileName,
	const Handle(AIS_InteractiveContext)& context) throw(FileError)
{
}

namespace
{

	FileType getFileType(QString& fileName)
	{
		QFileInfo fi(fileName);
		QString ext = fi.completeSuffix();

		if (ext == "brep" || ext == "rle")
			return ftBREP;

		if (ext == "csfdb")
			return ftCSFDB;

		if (ext == "iges")
			return ftIGES;

		if (ext == "step")
			return ftSTEP;

		if (ext == "stl")
			return ftSTL;

		if (ext == "vrml.gs")
			return ftVRML;

		return ftUnknown;
	}

	bool importModel(FileType ft, QString& fileName, Handle(TopTools_HSequenceOfShape)& shapes)
	{
		bool res = false;
		switch (ft)
		{
		case ftBREP:
			res = importBREP(fileName, shapes);
			break;
		case ftCSFDB:
			res = importCSFDB(fileName, shapes);
			break;
		case ftIGES:
			res = importIGES(fileName, shapes);
			break;
		case ftSTEP:
			res = importSTEP(fileName, shapes);
			break;
		default:
			break;
		}

		return res;
	}

	bool exportModel(FileType ft, QString& fileName, const Handle(AIS_InteractiveContext)& context)
	{
		Handle(TopTools_HSequenceOfShape) shapes = getShapes(context);
		if (shapes.IsNull() || !shapes->Length())
			return false;

		bool res;
		switch (ft)
		{
		case ftBREP:
			res = exportBREP(fileName, shapes);
			break;
		case ftCSFDB:
			res = exportCSFDB(fileName, shapes);
			break;
		case ftIGES:
			res = exportIGES(fileName, shapes);
			break;
		case ftSTEP:
			res = exportSTEP(fileName, shapes);
		break;
		case ftSTL:
			res = exportSTL(fileName, shapes);
			break;
		case ftVRML:
			res = exportVRML(fileName, shapes);
			break;
		default:
			res = false;
			break;
		}

		return res;
	}

	bool importBREP(QString& fileName, Handle(TopTools_HSequenceOfShape)& shapes)
	{
		TopoDS_Shape shape;
		BRep_Builder builder;

		Standard_Boolean result = BRepTools::Read(shape, (Standard_CString)fileName.toLatin1().constData(),
			builder);
		if (result)
			shapes->Append(shape);

		return result;
	}

	bool importIGES(QString& fileName, Handle(TopTools_HSequenceOfShape)& shapes)
	{
		IGESControl_Reader reader;
		int status = reader.ReadFile((Standard_CString)fileName.toLatin1().constData());

		if (status == IFSelect_RetDone)
		{
			reader.TransferRoots();
			TopoDS_Shape shape = reader.OneShape();
			shapes->Append(shape);
		}

		return (status == IFSelect_RetDone);
	}

	bool importSTEP(QString& fileName, Handle(TopTools_HSequenceOfShape)& shapes)
	{
		STEPControl_Reader reader;
		IFSelect_ReturnStatus status = reader.ReadFile((Standard_CString)fileName.toLatin1().constData());
		if (status == IFSelect_RetDone)
		{
			bool failsonly = false;
			reader.PrintCheckLoad(failsonly, IFSelect_ItemsByEntity);

			int nbr = reader.NbRootsForTransfer();
			reader.PrintCheckTransfer(failsonly, IFSelect_ItemsByEntity);
			for (Standard_Integer n = 1; n <= nbr; ++n)
			{
				reader.TransferRoot(n);
				int nbs = reader.NbShapes();
				if (nbs > 0)
				{
					for (int i = 1; i <= nbs; ++i)
					{
						TopoDS_Shape shape = reader.Shape(i);
						shapes->Append(shape);
					}
				}
			}
		}

		return (status == IFSelect_RetDone);
	}

	bool importCSFDB(QString& fileName, Handle(TopTools_HSequenceOfShape)& shapes)
	{
	    if (FSD_File::IsGoodFileType((Standard_CString)fileName.toLatin1().constData()) != Storage_VSOk)
		    return false;

	    static FSD_File fileDriver;
	    TCollection_AsciiString name((Standard_CString)fileName.toLatin1().constData());
	    if (fileDriver.Open(name, Storage_VSRead) != Storage_VSOk)
	        return false;

    	Handle(ShapeSchema) schema = new ShapeSchema();
	    Handle(Storage_Data) data  = schema->Read(fileDriver);
	    if (data->ErrorStatus() != Storage_VSOk)
	        return false;

    	fileDriver.Close();

    	Handle(Storage_HSeqOfRoot) roots = data->Roots();

	    for (int i = 1; i <= roots->Length(); ++i)
	    {
	        Handle(Storage_Root) r = roots->Value( i );
	        Handle(Standard_Persistent) p = r->Object();
	        Handle(PTopoDS_HShape) pShape = Handle(PTopoDS_HShape)::DownCast(p);
	        if (!pShape.IsNull() )
	        {
		        PTColStd_PersistentTransientMap map;
		        TopoDS_Shape tShape;
	            MgtBRep::Translate(pShape, map, tShape, MgtBRep_WithTriangle);
	            shapes->Append(tShape);
	        }
	    }

	    return true;
	}

	bool exportBREP(QString& fileName, Handle(TopTools_HSequenceOfShape)& shapes)
	{
		if (shapes.IsNull() || shapes->IsEmpty())
			return false;

		TopoDS_Shape shape = shapes->Value(1);
		return BRepTools::Write(shape, (Standard_CString)fileName.toLatin1().constData()); 
	}

	bool exportIGES(QString& fileName, Handle(TopTools_HSequenceOfShape)& shapes)
	{
		if (shapes.IsNull() || shapes->IsEmpty())
			return false;

		IGESControl_Controller::Init();
		IGESControl_Writer writer(Interface_Static::CVal("XSTEP.iges.unit"),
			Interface_Static::IVal("XSTEP.iges.writebrep.mode"));
 
		for (int i = 1; i <= shapes->Length(); ++i)
			writer.AddShape (shapes->Value(i));

		writer.ComputeModel();
		return writer.Write((Standard_CString)fileName.toLatin1().constData());
	}

	bool exportSTEP(QString& fileName, Handle(TopTools_HSequenceOfShape)& shapes)
	{
		/*if (shapes.IsNull() || shapes->IsEmpty())
			return false;

		//TranslateDlg* theDlg = getDialog(-1, false);
		STEPControl_StepModelType type;// = (STEPControl_StepModelType)theDlg->getMode();
		if (type < 0)
			return false;
    
		IFSelect_ReturnStatus status;

		if (type == STEPControl_FacetedBrep && !checkFacetedBrep(shapes))
		{
			return false;
		}

		STEPControl_Writer writer;
		for (int i = 1; i <= shapes->Length(); ++i)
		{
			status = writer.Transfer( shapes->Value(i), type);
			if (status != IFSelect_RetDone)
				return false;
		}

		status = writer.Write((Standard_CString)fileName.toLatin1().constData());

		return status == IFSelect_RetDone;*/

		return false;
	}

	bool exportCSFDB(QString& fileName, Handle(TopTools_HSequenceOfShape)& shapes)
	{
		return false;
	}

	bool exportSTL(QString& fileName, Handle(TopTools_HSequenceOfShape)& shapes)
	{
		return false;
	}

	bool exportVRML(QString& fileName, Handle(TopTools_HSequenceOfShape)& shapes)
	{
		return false;
	}

	Handle(TopTools_HSequenceOfShape) getShapes(const Handle(AIS_InteractiveContext)& context)
	{
		Handle(TopTools_HSequenceOfShape) sequence;
		Handle(AIS_InteractiveObject) picked;
		for (context->InitCurrent(); context->MoreCurrent(); context->NextCurrent())
		{
			Handle(AIS_InteractiveObject) obj = context->Current();
			if (obj->IsKind(STANDARD_TYPE(AIS_Shape)))
			{
				TopoDS_Shape shape = Handle(AIS_Shape)::DownCast(obj)->Shape();
				if (sequence.IsNull())
				sequence = new TopTools_HSequenceOfShape();
				sequence->Append(shape);
			}
		}
		return sequence;
	}

}

