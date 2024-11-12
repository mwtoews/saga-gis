
///////////////////////////////////////////////////////////
//                                                       //
//                         SAGA                          //
//                                                       //
//      System for Automated Geoscientific Analyses      //
//                                                       //
//                     Tool Library                      //
//                     climate_tools                     //
//                                                       //
//-------------------------------------------------------//
//                                                       //
//                   TLB_Interface.cpp                   //
//                                                       //
//                 Copyright (C) 2012 by                 //
//                      Olaf Conrad                      //
//                                                       //
//-------------------------------------------------------//
//                                                       //
// This file is part of 'SAGA - System for Automated     //
// Geoscientific Analyses'. SAGA is free software; you   //
// can redistribute it and/or modify it under the terms  //
// of the GNU General Public License as published by the //
// Free Software Foundation; version >=2 of the License. //
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
		return( _TL("Climate and Weather Tools") );

	case TLB_INFO_Category:
		return( _TL("Climate and Weather") );

	case TLB_INFO_Author:
		return( "O.Conrad (c) 2012" );

	case TLB_INFO_Description:
		return( _TL("Tools for the processing and analysis of climate and weather data.") );

	case TLB_INFO_Version:
		return( "1.0" );

	case TLB_INFO_Menu_Path:
		return( _TL("Climate and Weather") );
	}
}


//---------------------------------------------------------
// 3. Include the headers of your tools here...

#include "grid_levels_interpolation.h"
#include "milankovic.h"
#include "evapotranspiration.h"
#include "daily_sun.h"
#include "bioclimatic_vars.h"
#include "treeline.h"
#include "windeffect_correction.h"
#include "frost_change_frequency.h"
#include "thermal_belts.h"
#include "snow_cover.h"
#include "growing_degree_days.h"
#include "climate_classification.h"
#include "phenips.h"
#include "soil_water_balance.h"
#include "cloud_overlap.h"
#include "temperature_lapse_rates.h"
#include "air_pressure.h"
#include "land_surface_temperature.h"
#include "air_humidity.h"
#include "temperature_lapse_downscaling.h"


//---------------------------------------------------------
// 4. Allow your tools to be created here...

CSG_Tool *		Create_Tool(int i)
{
	switch( i )
	{
	case  0: 	return( new CGrid_Levels_to_Surface );
	case  1: 	return( new CGrid_Levels_to_Points );

	case  2:	return( new CMilankovic );
	case  3:	return( new CMilankovic_SR_Location );
	case  4:	return( new CMilankovic_SR_Day_Location );
	case  5:	return( new CMilankovic_SR_Monthly_Global );

	case  7:	return( new CETpot_Day_To_Hour );
	case  6:	return( new CETpot_Table );
	case  8:	return( new CETpot_Grid );

	case  9:	return( new CDaily_Sun );
	case 31:	return( new CSolarRadiation );

	case 10:	return( new CBioclimatic_Vars );
	case 11:	return( new CTree_Growth );
	case 20:	return( new CWater_Balance );
	case 12:	return( new CWater_Balance_Interactive );

	case 13:	return( new CWindeffect_Correction );

	case 14:	return( new CFrost_Change_Frequency );
	case 16:	return( new CFrost_Change_Frequency_Interactive );
	case 15:	return( new CThermal_Belts );

	case 17:	return( new CSnow_Cover );
	case 18:	return( new CGrowing_Degree_Days );

	case 19:	return( new CClimate_Classification );

	case 21:	return( new CPhenIps_Table );
	case 22:	return( new CPhenIps_Grids_Annual );
	case 23:	return( new CPhenIps_Grids_Days );

	case 24:	return( new CSoil_Water_Balance );

	case 25:	return( new CCloud_Overlap );
	case 26:	return( new CTemperature_Lapse_Rates );
	case 30:	return( new CTemperature_Lapse_Downscaling );
	case 32:	return( new CTemperature_Lapse_Interpolation );

	case 27:	return( new CAirPressure_Scaling );
	case 29:	return( new CAirHumidity_Conversion );

	case 28:	return( new CLand_Surface_Temperature );

	//-----------------------------------------------------
	case 33:	return( NULL );
	default:	return( TLB_INTERFACE_SKIP_TOOL );
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
