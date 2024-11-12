
///////////////////////////////////////////////////////////
//                                                       //
//                         SAGA                          //
//                                                       //
//      System for Automated Geoscientific Analyses      //
//                                                       //
//                     Tool Library                      //
//                 statistics_regression                 //
//                                                       //
//-------------------------------------------------------//
//                                                       //
//                   TLB_Interface.cpp                   //
//                                                       //
//                 Copyright (C) 2010 by                 //
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
		return( _TL("Regression") );

	case TLB_INFO_Category:
		return( _TL("Spatial and Geostatistics") );

	case TLB_INFO_Author:
		return( "O.Conrad (c) 2010" );

	case TLB_INFO_Description:
		return( _TL("Tools for correlation and regression analyses.") );

	case TLB_INFO_Version:
		return( "1.0" );

	case TLB_INFO_Menu_Path:
		return( _TL("Spatial and Geostatistics|Regression") );
	}
}


//---------------------------------------------------------
// 3. Include the headers of your tools here...

#include "point_grid_regression.h"
#include "point_multi_grid_regression.h"
#include "point_trend_surface.h"
#include "point_zonal_multi_grid_regression.h"

#include "gw_regression.h"
#include "gw_regression_grid.h"
#include "gw_multi_regression.h"
#include "gw_multi_regression_grid.h"
#include "gw_multi_regression_points.h"
#include "gwr_grid_downscaling.h"

#include "grid_multi_grid_regression.h"
#include "grids_trend_polynom.h"

#include "table_trend.h"
#include "table_regression_multiple.h"

#include "spatial_autocorrelation.h"


//---------------------------------------------------------
// 4. Allow your tools to be created here...

CSG_Tool *		Create_Tool(int i)
{
	switch( i )
	{
	case  0: return( new CPoint_Grid_Regression );
	case  1: return( new CPoint_Multi_Grid_Regression );
	case 15: return( new CPoint_Zonal_Multi_Grid_Regression );
	case  2: return( new CPoint_Trend_Surface );

	case  3: return( new CGW_Regression );
	case  4: return( new CGW_Regression_Grid );
	case  5: return( new CGW_Multi_Regression );
	case  6: return( new CGW_Multi_Regression_Grid );
	case  7: return( new CGW_Multi_Regression_Points );
	case 14: return( new CGWR_Grid_Downscaling );

	case  8: return( new CGrid_Multi_Grid_Regression );
	case  9: return( new CGrids_Trend );

	case 10: return( new CTable_Trend );
	case 11: return( new CTable_Trend_Shapes );

	case 12: return( new CTable_Regression_Multiple );
	case 13: return( new CTable_Regression_Multiple_Shapes );

	case 16: return( new CSpatial_Autocorrelation );

	//-----------------------------------------------------
	case 17: return( NULL );
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


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
double	GWR_Fit_To_Density(CSG_Shapes *pPoints, double Bandwidth, int Rounding)
{
	if( pPoints && pPoints->Get_Count() > 0 && pPoints->Get_Extent().Get_Area() > 0.0 )
	{
		double	d = sqrt(pPoints->Get_Extent().Get_Area() / pPoints->Get_Count());	// get a rough estimation of point density for band width suggestion

		if( Bandwidth > 0. )
		{
			d *= Bandwidth;
		}

		if( Rounding > 0 )
		{
			d  = SG_Get_Rounded_To_SignificantFigures(d, Rounding);
		}

		return( d );
	}

	return( -1. );
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
