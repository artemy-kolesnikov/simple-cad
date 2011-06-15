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

#include "filehelper.h"

#include <QFileInfo>
#include <QString>
#include <QDebug>

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
#include <IGESControl_Controller.hxx>
#include <Interface_Static.hxx>
#include <common/exception.h>

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
	bool importModel(FileType ft, QString& fileName, std::list<TopoDS_Shape>& shapes);
	bool exportModel(FileType ft, QString& fileName, const std::list<TopoDS_Shape>& shapes);

	bool importBREP(QString& fileName, std::list<TopoDS_Shape>& shapes);
	bool importIGES(QString& fileName, std::list<TopoDS_Shape>& shapes);
	bool importSTEP(QString& fileName, std::list<TopoDS_Shape>& shapes);
	bool importCSFDB(QString& fileName, std::list<TopoDS_Shape>& shapes);

	bool exportBREP(QString& fileName, const std::list<TopoDS_Shape>& shapes);
	bool exportIGES(QString& fileName, const std::list<TopoDS_Shape>& shapes);
	bool exportSTEP(QString& fileName, const std::list<TopoDS_Shape>& shapes);
	bool exportCSFDB(QString& fileName, const std::list<TopoDS_Shape>& shapes);
	bool exportSTL(QString& fileName, const std::list<TopoDS_Shape>& shapes);
	bool exportVRML(QString& fileName, const std::list<TopoDS_Shape>& shapes);
}

void FileHelper::readFile(QString& fileName,
	std::list<TopoDS_Shape>& shapes)
{
	FileType ft = getFileType(fileName);

    if (!importModel(ft, fileName, shapes))
	{
		throw Common::FileException(QObject::tr("Ошибка импорта файла"));
	}
}

void FileHelper::writeFile(QString& fileName,
	const std::list<TopoDS_Shape>& shapes)
{
	FileType ft = getFileType(fileName);

    if (!exportModel(ft, fileName, shapes))
	{
		throw Common::FileException(QObject::tr("Ошибка экспорта файла"));
	}
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

	bool importModel(FileType ft, QString& fileName, std::list<TopoDS_Shape>& shapes)
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

	bool exportModel(FileType ft, QString& fileName, const std::list<TopoDS_Shape>& shapes)
	{
		if (shapes.size() == 0)
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

	bool importBREP(QString& fileName, std::list<TopoDS_Shape>& shapes)
	{
		TopoDS_Shape shape;
		BRep_Builder builder;

		Standard_Boolean result = BRepTools::Read(shape, (Standard_CString)fileName.toLatin1().constData(),
			builder);
		if (result)
			shapes.push_back(shape);

		return result;
	}

	bool importIGES(QString& fileName, std::list<TopoDS_Shape>& shapes)
	{
		IGESControl_Reader reader;
		int status = reader.ReadFile((Standard_CString)fileName.toLatin1().constData());

		if (status == IFSelect_RetDone)
		{
			reader.TransferRoots();
			TopoDS_Shape shape = reader.OneShape();
			shapes.push_back(shape);
		}

		return (status == IFSelect_RetDone);
	}

	bool importSTEP(QString& fileName, std::list<TopoDS_Shape>& shapes)
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
						shapes.push_back(shape);
					}
				}
			}
		}

		return (status == IFSelect_RetDone);
	}

	bool importCSFDB(QString& fileName, std::list<TopoDS_Shape>& shapes)
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
	            shapes.push_back(tShape);
	        }
	    }

	    return true;
	}

	bool exportBREP(QString& fileName, const std::list<TopoDS_Shape>& shapes)
	{
		if (shapes.size() == 0)
			return false;

		const TopoDS_Shape& shape = shapes.front();
		return BRepTools::Write(shape, (Standard_CString)fileName.toLatin1().constData()); 
	}

	bool exportIGES(QString& fileName, const std::list<TopoDS_Shape>& shapes)
	{
		if (shapes.size() == 0)
			return false;

		IGESControl_Controller::Init();
		IGESControl_Writer writer(Interface_Static::CVal("XSTEP.iges.unit"),
			Interface_Static::IVal("XSTEP.iges.writebrep.mode"));
 
 		std::list<TopoDS_Shape>::const_iterator it = shapes.begin();
		for (;it != shapes.end(); ++it)
			writer.AddShape(*it);

		writer.ComputeModel();
		return writer.Write((Standard_CString)fileName.toLatin1().constData());
	}

	bool exportSTEP(QString& fileName, const std::list<TopoDS_Shape>& shapes)
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

	bool exportCSFDB(QString& fileName, const std::list<TopoDS_Shape>& shapes)
	{
		return false;
	}

	bool exportSTL(QString& fileName, const std::list<TopoDS_Shape>& shapes)
	{
		return false;
	}

	bool exportVRML(QString& fileName, const std::list<TopoDS_Shape>& shapes)
	{
		return false;
	}

}

