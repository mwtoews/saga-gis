
///////////////////////////////////////////////////////////
//                                                       //
//                         SAGA                          //
//                                                       //
//      System for Automated Geoscientific Analyses      //
//                                                       //
//                     Tool Library                      //
//                      dev_tools                        //
//                                                       //
//-------------------------------------------------------//
//                                                       //
//                   TLB_Interface.cpp                   //
//                                                       //
//                 Copyright (C) 2010 by                 //
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
//                Institute for Geography                //
//                University of Hamburg                  //
//                Germany                                //
//                                                       //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
// 1. Include the appropriate SAGA-API header...

#include <saga_api/saga_api.h>


//---------------------------------------------------------
// 2. Place general tool library information here...

CSG_String Get_Info(int i)
{
	switch( i )
	{
	case TLB_INFO_Name:	default:
		return( _TL("Development Tools") );

	case TLB_INFO_Category:
		return( "SAGA Development" );

	case TLB_INFO_Author:
		return( "O. Conrad (c) 2010" );

	case TLB_INFO_Description:
		return( _TL("Tools and helpers for the SAGA development.") );

	case TLB_INFO_Version:
		return( "1.0" );

	case TLB_INFO_Menu_Path:
		return( "SAGA Development|Development Tools" );
	}
}


//---------------------------------------------------------
// 3. Include the headers of your tools here...

#include "tl_extract.h"
#include "tl_union.h"
#include "tl_merge.h"
#include "tool_counter.h"
#include "arctoolbox.h"
#include "qgistoolbox.h"
#include "svg_converter.h"
#include "pysaga_tools.h"
#include "tool_description.h"
#include "pretty_printer_tool.h"


//---------------------------------------------------------
// 4. Allow your tools to be created here...

CSG_Tool *		Create_Tool(int i)
{
	switch( i )
	{
	case  0: return( new CTL_Extract );
	case  1: return( new CTL_Union );
	case  2: return( new CTL_Merge );
	case  3: return( new CTool_Counter );
	case  4: return( new CTool_Menus );
	case  5: return( new CArcToolBox );
	case  7: return( new CQGIS_ToolBox );
	case  8: return( new CSVG_Converter );
	case  9: return( new CPySAGA_Tools );
	case 10: return( new CTool_Description );
	case 11: return( new CPrettyPrinter );

	case 12: return( NULL );
	default: return( TLB_INTERFACE_SKIP_TOOL );
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
