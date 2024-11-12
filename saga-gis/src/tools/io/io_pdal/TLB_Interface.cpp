
///////////////////////////////////////////////////////////
//                                                       //
//                         SAGA                          //
//                                                       //
//      System for Automated Geoscientific Analyses      //
//                                                       //
//                     Tool Library                      //
//                       io_pdal                         //
//                                                       //
//-------------------------------------------------------//
//                                                       //
//                   TLB_Interface.cpp                   //
//                                                       //
//                 Copyrights (C) 2020                   //
//                     Olaf Conrad                       //
//                                                       //
//-------------------------------------------------------//
//                                                       //
// This file is part of 'SAGA - System for Automated     //
// Geoscientific Analyses'. SAGA is free software; you   //
// can redistribute it and/or modify it under the terms  //
// of the GNU General Public License as published by the //
// Free Software Foundation, either version 2 of the     //
// License, or (at your option) any later version.       //
//                                                       //
// SAGA is distributed in the hope that it will be       //
// useful, but WITHOUT ANY WARRANTY; without even the    //
// implied warranty of MERCHANTABILITY or FITNESS FOR A  //
// PARTICULAR PURPOSE. See the GNU General Public        //
// License for more details.                             //
//                                                       //
// You should have received a copy of the GNU General    //
// Public License along with this program; if not, see   //
// <http://www.gnu.org/licenses/>.                       //
//                                                       //
//-------------------------------------------------------//
//                                                       //
//    e-mail:     oconrad@saga-gis.org                   //
//                                                       //
//    contact:    Olaf Conrad                            //
//                Institute of Geography                 //
//                University of Hamburg                  //
//                Germany                                //
//                                                       //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
// 1. Include the appropriate SAGA-API header...

#include "pdal_driver.h"


//---------------------------------------------------------
// 2. Place general module library information here...

CSG_String Get_Info(int i)
{
	switch( i )
	{
	case TLB_INFO_Name:	default:
		return( _TL("PDAL") );

	case TLB_INFO_Category:
		return( "Import/Export" );

	case TLB_INFO_Author:
		return( "O.Conrad, V. Wichmann (c) 2020-2021" );

	case TLB_INFO_Description:
		return( CSG_String::Format(_TL("Tools that use the Point Data Abstraction Library (PDAL)."))
			+ CSG_String::Format("\n\nPDAL %s: ", _TL("Version")) + SG_Get_PDAL_Drivers().Get_Version()
		);

	case TLB_INFO_Version:
		return( "1.0" );

	case TLB_INFO_Menu_Path:
		return( "File|Shapes" );
	}
}


//---------------------------------------------------------
// 3. Include the headers of your modules here...

#include "pdal_reader.h"
#include "pdal_writer_las.h"
#include "pdal_to_grid.h"


//---------------------------------------------------------
// 4. Allow your modules to be created here...

CSG_Tool *		Create_Tool(int i)
{
	switch( i )
	{
	default: return( TLB_INTERFACE_SKIP_TOOL );

	case  0: return( new CPDAL_Reader );
    case  1: return( new CPDAL_Writer_Las );
	case  2: return( new CPDAL_to_Grid );

	//-----------------------------------------------------
	case 10: return( NULL );
	}
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
//{{AFX_SAGA

	TLB_INTERFACE

//}}AFX_SAGA
