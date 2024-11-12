
///////////////////////////////////////////////////////////
//                                                       //
//                         SAGA                          //
//                                                       //
//      System for Automated Geoscientific Analyses      //
//                                                       //
//                     Tool Library                      //
//                     Grid_Calculus                     //
//                                                       //
//-------------------------------------------------------//
//                                                       //
//                   TLB_Interface.cpp                   //
//                                                       //
//                 Copyright (C) 2003 by                 //
//               SAGA User Group Associaton              //
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
//    contact:    SAGA User Group Associaton             //
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
		return( _TL("Calculus") );

	case TLB_INFO_Category:
		return( _TL("Grid") );

	case TLB_INFO_Author:
		return( "O. Conrad, A. Ringeler, V. Olaya, J. Engels (c) 2001-2018" );

	case TLB_INFO_Description:
		return( _TL("Grid based or related calculations.") );

	case TLB_INFO_Version:
		return( "1.0" );

	case TLB_INFO_Menu_Path:
		return( _TL("Grid|Calculus" ));
	}
}


//---------------------------------------------------------
// 3. Include the headers of your tools here...

#include "Grid_Normalise.h"
#include "Grid_Calculator.h"
#include "Grid_Volume.h"
#include "grid_difference.h"
#include "Grid_Plotter.h"
#include "Grid_Geometric_Figures.h"
#include "Grid_Random_Terrain.h"
#include "Grid_Random_Field.h"

#include "Fuzzify.h"
#include "FuzzyAND.h"
#include "FuzzyOR.h"

#include "grid_metric_conversion.h"
#include "gradient_cartes_polar.h"

#include "kff_synth.h"

#include "grid_histogram_match.h"


//---------------------------------------------------------
// 4. Allow your tools to be created here...

CSG_Tool *		Create_Tool(int i)
{
	switch( i )
	{
	case  1: return( new CGrid_Calculator );
	case  2: return( new CGrid_Volume );

	case  4: return( new CGrid_Plotter );
	case  5: return( new CGrid_Geometric_Figures );
	case  6: return( new CGrid_Random_Terrain );
	case  7: return( new CGrid_Random_Field );
	case 17: return( new CGrid_Fractal_Brownian_Noise );

	case  0: return( new CGrid_Normalise );
	case 10: return( new CGrid_Standardise );

	case 22: return( new CGrid_Addition );
	case  3: return( new CGrid_Subtraction );
	case 23: return( new CGrid_Multiplication );
	case 18: return( new CGrid_Division );
	case  8: return( new CGrids_Sum );
	case  9: return( new CGrids_Product );

	case 11: return( new CFuzzify );
	case 12: return( new CFuzzyAND );
	case 13: return( new CFuzzyOR );

	case 14: return( new CGrid_Metric_Conversion );

	case 15: return( new CGradient_Cartes_To_Polar );
	case 16: return( new CGradient_Polar_To_Cartes );

	case 19: return( new Ckff_synthesis );

	case 20: return( new CGrids_Calculator );

	case 21: return( new CGrid_Histogram_Match );

	//-----------------------------------------------------
	case 24: return( NULL );
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
