
///////////////////////////////////////////////////////////
//                                                       //
//                         SAGA                          //
//                                                       //
//      System for Automated Geoscientific Analyses      //
//                                                       //
//                     Tool Library                      //
//                       io_shapes                       //
//                                                       //
//-------------------------------------------------------//
//                                                       //
//                   TLB_Interface.cpp                   //
//                                                       //
//                 Copyright (C) 2003 by                 //
//                      Olaf Conrad                      //
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
//                University of Goettingen               //
//                Goldschmidtstr. 5                      //
//                37077 Goettingen                       //
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
		return( _TL("Shapes") );

	case TLB_INFO_Category:
		return( _TL("Import/Export") );

	case TLB_INFO_Author:
		return( "SAGA User Group Associaton (c) 2002-23" );

	case TLB_INFO_Description:
		return( _TL("Tools for the import and export of vector data.") );

	case TLB_INFO_Version:
		return( "1.0" );

	case TLB_INFO_Menu_Path:
		return( _TL("File|Shapes") );
	}
}


//---------------------------------------------------------
// 3. Include the headers of your tools here...

#include "gstat.h"
#include "xyz.h"
#include "generate.h"
#include "surfer_bln.h"
#include "atlas_bna.h"
#include "wasp_map.h"
#include "stl.h"
#include "gpx.h"
#include "pointcloud_from_file.h"
#include "pointcloud_from_text_file.h"
#include "svg.h"
#include "pointcloud_to_text_file.h"
#include "wktb.h"
#include "citygml_import.h"
#include "html_imagemap.h"
#include "pts.h"
#include "aseg_gdf.h"


//---------------------------------------------------------
// 4. Allow your tools to be created here...

CSG_Tool *		Create_Tool(int i)
{
	switch( i )
	{
	case  0: return( new CGStat_Export );
	case  1: return( new CGStat_Import );
	case  2: return( new CXYZ_Export );
	case  3: return( new CXYZ_Import );
	case  4: return( new CGenerate_Export );
	case  5: return( new CSurfer_BLN_Export );
	case  6: return( new CSurfer_BLN_Import );
	case  7: return( new CAtlas_BNA_Export );
	case  8: return( new CAtlas_BNA_Import );
	case  9: return( new CWASP_MAP_Export );
	case 10: return( new CWASP_MAP_Import );
	case 11: return( new CSTL_Import );
	case 12: return( new CSTL_Export );
	case 13: return( new CGPX_Import );
	case 14: return( new CGPX_Export );
	case 15: return( new CPointCloud_From_File );
	case 16: return( new CPointCloud_From_Text_File );
	case 17: return( new CSVG_Export );
	case 18: return( new CPointcloud_To_Text_File );
	case 19: return( new CWKT_Import );
	case 20: return( new CWKT_Export );
	case 21: return( new CCityGML_Import );
	case 22: return( new CHTML_ImageMap );
	case 23: return( new CPTS_Import );
	case 24: return( new CASEG_GDF_Import );

	//-----------------------------------------------------
	case 25: return( NULL );
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
