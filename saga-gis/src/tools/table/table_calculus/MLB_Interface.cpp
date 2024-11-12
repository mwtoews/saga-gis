
///////////////////////////////////////////////////////////
//                                                       //
//                         SAGA                          //
//                                                       //
//      System for Automated Geoscientific Analyses      //
//                                                       //
//                     Tool Library                      //
//                    Table_Calculus                     //
//                                                       //
//-------------------------------------------------------//
//                                                       //
//                   TLB_Interface.cpp                   //
//                                                       //
//                 Copyright (C) 2023 by                 //
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
		return( _TL("Calculus") );

	case TLB_INFO_Category:
		return( _TL("Table") );

	case TLB_INFO_Author:
		return( "SAGA User Group Associaton (c) 2002-2023" );

	case TLB_INFO_Description:
		return( _TL("Tools for table based analyses and calculations.") );

	case TLB_INFO_Version:
		return( "1.0" );

	case TLB_INFO_Menu_Path:
		return( _TL("Table|Calculus") );
	}
}


//---------------------------------------------------------
// 3. Include the headers of your tools here...

#include "Fit.h"
#include "Table_Calculator.h"
#include "table_running_average.h"
#include "table_pca.h"
#include "table_fill_record_gaps.h"
#include "table_field_analyzer.h"
#include "table_mRMR.h"
#include "table_field_statistics.h"
#include "table_record_statistics.h"
#include "table_aggregate_by_field.h"
#include "confusion_matrix.h"


//---------------------------------------------------------
// 4. Allow your tools to be created here...

CSG_Tool *		Create_Tool(int i)
{
	switch( i )
	{
	case  0: return( new CFit );

	case  1: return( new CTable_Field_Calculator );

	case  5: return( new CTable_Running_Average );
	case  7: return( new CTable_PCA );

	case  8: return( new CTable_Insert_Records );
	case  9: return( new CTable_Fill_Record_Gaps );

	case 11: return( new CTable_Field_Extreme );

#ifdef WITH_MRMR
	case 12: return( new CTable_mRMR );
#endif // WITH_MRMR

	case 15: return( new CTable_Field_Statistics );
	case 16: return( new CTable_Record_Statistics );

	case 18: return( new CTable_Aggregate_by_Field() );

	case 19: return( new CConfusion_Matrix() );

	case 20: return( new CTable_Aggregation );

	//-----------------------------------------------------
	case 21: return( NULL );
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
