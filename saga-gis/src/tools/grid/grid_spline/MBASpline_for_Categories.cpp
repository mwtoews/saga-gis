
///////////////////////////////////////////////////////////
//                                                       //
//                         SAGA                          //
//                                                       //
//      System for Automated Geoscientific Analyses      //
//                                                       //
//                     Tool Library                      //
//                     grid_spline                       //
//                                                       //
//-------------------------------------------------------//
//                                                       //
//               MBASpline_for_Categories.cpp            //
//                                                       //
//                 Copyright (C) 2015 by                 //
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
#include "MBASpline_for_Categories.h"


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
CMBASpline_for_Categories::CMBASpline_for_Categories(void)
{
	Set_Name		(_TL("Multilevel B-Spline for Categories"));

	Set_Author		("O.Conrad (c) 2015");

	Set_Description	(_TW(
		"The 'Multilevel B-Spline for Categories' tool is comparable to "
		"indicator Kriging except that uses the Multilevel B-spline "
		"algorithm for interpolation. "
	));

	Add_Reference(
		"Lee, S., Wolberg, G., Shin, S.Y.", "1997",
		"Scattered Data Interpolation with Multilevel B-Splines",
		"IEEE Transactions On Visualisation And Computer Graphics, Vol.3, No.3., p.228-244.",
		SG_T("https://www.researchgate.net/profile/George_Wolberg/publication/3410822_Scattered_Data_Interpolation_with_Multilevel_B-Splines/links/00b49518719ac9f08a000000/Scattered-Data-Interpolation-with-Multilevel-B-Splines.pdf"),
		SG_T("ResearchGate")
	);

	//-----------------------------------------------------
	Parameters.Add_Shapes("",
		"POINTS", _TL("Points"),
		_TL(""),
		PARAMETER_INPUT
	);

	Parameters.Add_Table_Field("POINTS",
		"FIELD" , _TL("Attribute"),
		_TL("")
	);

	//-----------------------------------------------------
	m_Grid_Target.Create(&Parameters, false, "", "TARGET_");

	m_Grid_Target.Add_Grid("CATEGORIES" , _TL("Categories" ), false);
	m_Grid_Target.Add_Grid("PROBABILITY", _TL("Probability"), false);
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
int CMBASpline_for_Categories::On_Parameter_Changed(CSG_Parameters *pParameters, CSG_Parameter *pParameter)
{
	if( pParameter->Cmp_Identifier("POINTS") )
	{
		m_Grid_Target.Set_User_Defined(pParameters, pParameter->asShapes());
	}

	m_Grid_Target.On_Parameter_Changed(pParameters, pParameter);

	return( CSG_Tool::On_Parameter_Changed(pParameters, pParameter) );
}

//---------------------------------------------------------
int CMBASpline_for_Categories::On_Parameters_Enable(CSG_Parameters *pParameters, CSG_Parameter *pParameter)
{
	m_Grid_Target.On_Parameters_Enable(pParameters, pParameter);

	return( CSG_Tool::On_Parameters_Enable(pParameters, pParameter) );
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CMBASpline_for_Categories::On_Execute(void)
{
	CSG_Shapes	Points;

	//-----------------------------------------------------
	SG_RUN_TOOL_ExitOnError("table_tools", 20,	// Add Indicator Fields for Categories
			SG_TOOL_PARAMETER_SET("TABLE"     , Parameters("POINTS")->asShapes())
		&&	SG_TOOL_PARAMETER_SET("FIELD"     , Parameters("FIELD"))
		&&	SG_TOOL_PARAMETER_SET("OUT_SHAPES", &Points)	// >> Indicators
	)

	int	nCategories	= Points.Get_Field_Count() - 1;

	if( nCategories < 2 )
	{
		Error_Set(_TL("found less than two categories, nothing to do"));

		return( false );
	}

	//-----------------------------------------------------
	CSG_Grid	*pGrid, *pProp, Prop;

	if( !(pGrid = m_Grid_Target.Get_Grid("CATEGORIES", nCategories < 128 ? SG_DATATYPE_Char : SG_DATATYPE_Int))
	||  !(pProp = m_Grid_Target.Get_Grid("PROBABILITY")) )
	{
		return( false );
	}

	//-----------------------------------------------------
	CSG_Parameter	*pLUT	= DataObject_Get_Parameter(pGrid, "LUT");

	if( pLUT && pLUT->asTable() )
	{
		CSG_Colors	Colors(nCategories);	Colors.Random();

		CSG_Table	*pClasses	= pLUT->asTable();

		pClasses->Set_Count(nCategories);

		for(int iClass=0; iClass<nCategories; iClass++)
		{
			CSG_Table_Record	*pClass	= pClasses->Get_Record(iClass);

			pClass->Set_Value(0, Colors[iClass]);
			pClass->Set_Value(1, Points.Get_Field_Name(1 + iClass));
			pClass->Set_Value(2, "");
			pClass->Set_Value(3, iClass);
			pClass->Set_Value(4, iClass);
		}

		DataObject_Set_Parameter(pGrid, pLUT);	// Lookup Table
		DataObject_Set_Parameter(pGrid, "COLORS_TYPE", 1);	// Color Classification Type: Lookup Table
	}

	//-----------------------------------------------------
	pGrid->Fmt_Name("%s"     , Points.Get_Field_Name(0));
	pProp->Fmt_Name("%s [%s]", Points.Get_Field_Name(0), _TL("Probability"));

	pProp->Assign(0.0);
	pProp->Set_NoData_Value(0.0);

	Prop.Create(pGrid->Get_System());

	//-----------------------------------------------------
	for(int i=0; i<nCategories; i++)
	{
		Process_Set_Text("%s: %s", _TL("processing"), Points.Get_Field_Name(1 + i));

		SG_UI_Progress_Lock(true);

		SG_RUN_TOOL_ExitOnError("grid_spline", 4,	// Multilevel B-Spline Interpolation
				SG_TOOL_PARAMETER_SET("SHAPES"           , &Points)
			&&	SG_TOOL_PARAMETER_SET("FIELD"            , 1 + i)	// indicator field
			&&	SG_TOOL_PARAMETER_SET("TARGET_DEFINITION", 1)		// grid or grid system
			&&	SG_TOOL_PARAMETER_SET("TARGET_OUT_GRID"  , &Prop)	// target grid
		)

		SG_UI_Progress_Lock(false);

		#pragma omp parallel for
		for(int y=0; y<pGrid->Get_NY(); y++)
		{
			for(int x=0; x<pGrid->Get_NX(); x++)
			{
				if( pProp->asDouble(x, y) < Prop.asDouble(x, y) )
				{
					pProp->Set_Value(x, y, Prop.asDouble(x, y));
					pGrid->Set_Value(x, y, i);
				}
			}
		}
	}

	//-----------------------------------------------------
	return( true );
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
