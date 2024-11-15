
///////////////////////////////////////////////////////////
//                                                       //
//                         SAGA                          //
//                                                       //
//      System for Automated Geoscientific Analyses      //
//                                                       //
//                     Tool Library                      //
//                    ta_morphometry                     //
//                                                       //
//-------------------------------------------------------//
//                                                       //
//                fuzzy_landform_elements.h              //
//                                                       //
//                 Copyright (C) 2013 by                 //
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
#include "fuzzy_landform_elements.h"


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
enum
{
	SLOPE	= 0,
	MINCURV,
	MAXCURV,
	PCURV,
	TCURV,
	IN_COUNT
};

//---------------------------------------------------------
const CSG_String	IN_Type[IN_COUNT][2]	= 
{
	{	"SLOPE"     , _TL("Slope"               )	},
	{	"MINCURV"   , _TL("Minimum Curvature"   )	},
	{	"MAXCURV"   , _TL("Maximum Curvature"   )	},
	{	"PCURV"     , _TL("Profile Curvature"   )	},
	{	"TCURV"     , _TL("Tangential Curvature")	}
};


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
struct SForm_Def
{
	CSG_String	ID, Name;

	long		Color;

	int			Value;
};

//---------------------------------------------------------
enum
{
	PLAIN	= 0,
	PIT,
	PEAK,
	RIDGE,
	CHANNEL,
	SADDLE,
	BSLOPE,
	FSLOPE,
	SSLOPE,
	HOLLOW,
	FHOLLOW,
	SHOLLOW,
	SPUR,
	FSPUR,
	SSPUR,
	FE_COUNT
};

//---------------------------------------------------------
const struct SForm_Def Form_Def[FE_COUNT]	= 
{
	{	"PLAIN"     , _TL("Plain"          ), SG_GET_RGB(220, 220, 220), 100	},
	{	"PIT"       , _TL("Pit"            ), SG_GET_RGB(  0,   0, 100), 111	},
	{	"PEAK"      , _TL("Peak"           ), SG_GET_RGB(100,   0,   0), 122	},
	{	"RIDGE"     , _TL("Ridge"          ), SG_GET_RGB(200,   0,   0), 120	},
	{	"CHANNEL"   , _TL("Channel"        ), SG_GET_RGB(  0,   0, 255), 101	},
	{	"SADDLE"    , _TL("Saddle"         ), SG_GET_RGB(  0, 200, 255), 121	},
	{	"BSLOPE"    , _TL("Back Slope"     ), SG_GET_RGB(255, 255,   0),   0	},
	{	"FSLOPE"    , _TL("Foot Slope"     ), SG_GET_RGB(240, 220,   0),  10	},
	{	"SSLOPE"    , _TL("Shoulder Slope" ), SG_GET_RGB(255, 255, 150),  20	},
	{	"HOLLOW"    , _TL("Hollow"         ), SG_GET_RGB(180, 230,   0),   1	},
	{	"FHOLLOW"   , _TL("Foot Hollow"    ), SG_GET_RGB(100, 230,   0),  11	},
	{	"SHOLLOW"   , _TL("Shoulder Hollow"), SG_GET_RGB(180, 255,   0),  21	},
	{	"SPUR"      , _TL("Spur"           ), SG_GET_RGB(250, 210,   0),   2	},
	{	"FSPUR"     , _TL("Foot Spur"      ), SG_GET_RGB(250, 170,   0),  12	},
	{	"SSPUR"     , _TL("Shoulder Spur"  ), SG_GET_RGB(255, 120,   0),  22	}
};


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
CFuzzy_Landform_Elements::CFuzzy_Landform_Elements(void)
{
	Set_Name		(_TL("Fuzzy Landform Element Classification"));

	Set_Author		("O.Conrad (c) 2013");

	Set_Description	(_TW(
		"Algorithm for derivation of form elements according to slope, maximum curvature, "
		"minimum curvature, profile curvature, tangential curvature, "
		"based on a linear semantic import model for slope and curvature and a fuzzy classification "
		"Based on the AML script \'felementf\' by Jochen Schmidt, Landcare Research. "
	));

	Add_Reference("Schmidt, J. & Hewitt, A.", "2004",
		"Fuzzy land element classification from DTMs based on geometry and terrain position",
		"Geoderma, 121(3-4), 243-256.",
		SG_T("https://doi.org/10.1016/j.geoderma.2003.10.008"), SG_T("doi:10.1016/j.geoderma.2003.10.008")
	);

	//-----------------------------------------------------
	Parameters.Add_Grid("", "ELEVATION", _TL("Elevation"         ), _TL(""), PARAMETER_INPUT);

	for(int i=0; i<IN_COUNT; i++)
	{
		Parameters.Add_Grid("", IN_Type [i][0], IN_Type[i][1]     , _TL(""), PARAMETER_INPUT);
	}

	Parameters.Add_Grid("", "FORM"     , _TL("Landform"          ), _TL(""), PARAMETER_OUTPUT, true, SG_DATATYPE_Char);
	Parameters.Add_Grid("", "MEM"      , _TL("Maximum Membership"), _TL(""), PARAMETER_OUTPUT_OPTIONAL);
	Parameters.Add_Grid("", "ENTROPY"  , _TL("Entropy"           ), _TL(""), PARAMETER_OUTPUT_OPTIONAL);
	Parameters.Add_Grid("", "CI"       , _TL("Confusion Index"   ), _TL(""), PARAMETER_OUTPUT_OPTIONAL);

	for(int i=0; i<FE_COUNT; i++)
	{
		Parameters.Add_Grid("", Form_Def[i].ID, Form_Def[i].Name  , _TL(""), PARAMETER_OUTPUT_OPTIONAL);
	}

	//-----------------------------------------------------
	Parameters.Add_Choice("",
		"INPUT"		, _TL("Input"),
		_TL("if elevation is selected, slopes and curvatures will be calculated internally"),
		CSG_String::Format("%s|%s",
			_TL("elevation"),
			_TL("slope and curvatures")
		), 0
	);

	Parameters.Add_Bool("",
		"MEMBERSHIP", _TL("Memberships"),
		_TL(""),
		false
	);

	Parameters.Add_Choice("SLOPE",
		"SLOPETODEG", _TL("Slope Grid Units"),
		_TL(""),
		CSG_String::Format("%s|%s",
			_TL("degree"),
			_TL("radians")
		), m_bToDegree ? 1 : 0
	);

	Parameters.Add_Range("",
		"T_SLOPE"	, _TL("Slope Thresholds [Degree]"),
		_TL("lower and upper thresholds for semantic import model, planar vs. sloped areas"),
		m_loSlope, m_hiSlope, 0., true, 90., true
	);

	Parameters.Add_Range("",
		"T_CURVE"	, _TL("Curvature Thresholds [1000 / m]"),
		_TL("lower and upper thresholds for semantic import model, straight vs. curved areas"),
		m_loCurve, m_hiCurve
	);
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
int CFuzzy_Landform_Elements::On_Parameters_Enable(CSG_Parameters *pParameters, CSG_Parameter *pParameter)
{
	if( pParameter->Cmp_Identifier("INPUT") )
	{
		for(int i=0; i<IN_COUNT; i++)
		{
			pParameters->Set_Enabled(IN_Type[i][0], pParameter->asInt() == 1);
		}

		pParameters->Set_Enabled("ELEVATION", pParameter->asInt() == 0);
	}

	if( pParameter->Cmp_Identifier("MEMBERSHIP") )
	{
		for(int i=0; i<FE_COUNT; i++)
		{
			pParameters->Set_Enabled(Form_Def[i].ID, pParameter->asBool());
		}
	}

	return( CSG_Tool_Grid::On_Parameters_Enable(pParameters, pParameter) );
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CFuzzy_Landform_Elements::On_Execute(void)
{
	CSG_Grid *pInput[IN_COUNT], _Input[IN_COUNT], *pMembership[FE_COUNT];

	//-----------------------------------------------------
	if( Parameters("INPUT")->asInt() == 0 )	// elevation
	{
		CSG_Grid Aspect(Get_System());

		for(int i=0; i<IN_COUNT; i++)
		{
			_Input[i].Create(Get_System());

			pInput[i] = &_Input[i];
		}

		CSG_Tool *pTool = SG_Get_Tool_Library_Manager().Create_Tool("ta_morphometry", 0);	// Slope, Aspect, Curvatures

		pTool->Set_Manager(NULL);

		if( !pTool
		||  !pTool->Set_Parameter("ELEVATION" , Parameters("ELEVATION"))
		||  !pTool->Set_Parameter("ASPECT"    , &Aspect)
		||  !pTool->Set_Parameter("SLOPE"     , pInput[SLOPE  ])
		||  !pTool->Set_Parameter("C_PROF"    , pInput[PCURV  ])
		||  !pTool->Set_Parameter("C_TANG"    , pInput[TCURV  ])
		||  !pTool->Set_Parameter("C_MINI"    , pInput[MINCURV])
		||  !pTool->Set_Parameter("C_MAXI"    , pInput[MAXCURV])
		||  !pTool->Set_Parameter("UNIT_SLOPE", 1)	// degree
		||  !pTool->Execute() )
		{
			SG_Get_Tool_Library_Manager().Delete_Tool(pTool);

			return( false );
		}

		SG_Get_Tool_Library_Manager().Delete_Tool(pTool);

		m_bToDegree	= false;
	}
	else
	{
		for(int i=0; i<IN_COUNT; i++)
		{
			pInput[i] = Parameters(IN_Type[i][0])->asGrid();
		}

		m_bToDegree = Parameters("SLOPETODEG")->asInt() == 1;
	}

	//-----------------------------------------------------
	m_loSlope = Parameters("T_SLOPE.MIN")->asDouble();// * M_DEG_TO_RAD;
	m_hiSlope = Parameters("T_SLOPE.MAX")->asDouble();// * M_DEG_TO_RAD;

	m_loCurve = Parameters("T_CURVE.MIN")->asDouble() / 1000.;
	m_hiCurve = Parameters("T_CURVE.MAX")->asDouble() / 1000.;

	//-----------------------------------------------------
	CSG_Grid *pForm    = Parameters("FORM"   )->asGrid();
	CSG_Grid *pMem     = Parameters("MEM"    )->asGrid();
	CSG_Grid *pEntropy = Parameters("ENTROPY")->asGrid();
	CSG_Grid *pCI      = Parameters("CI"     )->asGrid();

	for(int i=0; i<FE_COUNT; i++)
	{
		pMembership[i] = Parameters("MEMBERSHIP")->asBool() ? Parameters(Form_Def[i].ID)->asGrid() : NULL;

		DataObject_Set_Colors(pMembership[i], 11, SG_COLORS_WHITE_RED);
	}

	//-----------------------------------------------------
	CSG_Parameter *pLUT = DataObject_Get_Parameter(pForm, "LUT");

	if( pLUT && pLUT->asTable() )
	{
		pLUT->asTable()->Del_Records();

		for(int i=0; i<FE_COUNT; i++)
		{
			CSG_Table_Record	*pRecord	= pLUT->asTable()->Add_Record();

			pRecord->Set_Value(0, Form_Def[i].Color);
			pRecord->Set_Value(1, Form_Def[i].Name );
			pRecord->Set_Value(3, Form_Def[i].Value);
			pRecord->Set_Value(4, Form_Def[i].Value);
		}

		DataObject_Set_Parameter(pForm, pLUT);

		DataObject_Set_Parameter(pForm, "COLORS_TYPE", 1);	// Color Classification Type: Lookup Table
	}

	//-----------------------------------------------------
	for(int y=0; y<Get_NY() && Set_Progress_Rows(y); y++)
	{
		#pragma omp parallel for
		for(int x=0; x<Get_NX(); x++)
		{
			bool bOkay = true; double Input[IN_COUNT];

			for(int i=0; bOkay && i<IN_COUNT; i++)
			{
				if( (bOkay = !pInput[i]->is_NoData(x, y)) == true )
				{
					Input[i] = pInput[i]->asDouble(x, y);
				}
			}

			int Element; double MaxMem, Entropy, CI, Membership[FE_COUNT];

			if( bOkay && Get_Memberships(Input, Membership, Element, MaxMem, Entropy, CI) )
			{
				if( pForm    ) pForm   ->Set_Value(x, y, Element);	// hard classification according to highest membership value
				if( pMem     ) pMem    ->Set_Value(x, y, MaxMem );
				if( pEntropy ) pEntropy->Set_Value(x, y, Entropy);
				if( pCI      ) pCI     ->Set_Value(x, y, CI     );	// confusion index

				for(int i=0; i<FE_COUNT; i++)
				{
					if( pMembership[i] ) pMembership[i]->Set_Value(x, y, Membership[i]);
				}
			}
			else
			{
				if( pForm    ) pForm   ->Set_NoData(x, y);
				if( pMem     ) pMem    ->Set_NoData(x, y);
				if( pEntropy ) pEntropy->Set_NoData(x, y);
				if( pCI      ) pCI     ->Set_NoData(x, y);

				for(int i=0; i<FE_COUNT; i++)
				{
					if( pMembership[i] ) pMembership[i]->Set_NoData(x, y);
				}
			}
		}
	}

	//-----------------------------------------------------
	return( true );
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#define GET_MINIMUM(a, b, c)	M_GET_MIN(a, M_GET_MIN(b, c))

//---------------------------------------------------------
bool CFuzzy_Landform_Elements::Get_Memberships(double Input[], double M[], int &Element, double &MaxMem, double &Entropy, double &CI)
{
	double	bFlat, bSlope, MaxMem2,
			bPCurve  , bPCurveX  , bPCurveV,
			bTCurve  , bTCurveX  , bTCurveV,
			bMaxCurve, bMaxCurveX, bMaxCurveV,
			bMinCurve, bMinCurveX, bMinCurveV;

	//-----------------------------------------------------
	if( m_bToDegree )
	{
		Input[SLOPE]	*= M_RAD_TO_DEG;
	}

	//-----------------------------------------------------
	// membership slope flat
	bFlat		= Input[SLOPE] <= m_loSlope ? 1.0 : (Input[SLOPE] >= m_hiSlope ? 0.0
				: (m_hiSlope - Input[SLOPE]) / (m_hiSlope - m_loSlope));			// linear membership fct
	bSlope		= 1 - bFlat;

	// membership pcurv straight
	bPCurve		= fabs(Input[PCURV]) <= m_loCurve ? 1.0 : (fabs(Input[PCURV]) >= m_hiCurve ? 0.0
				: (m_hiCurve - fabs(Input[PCURV])) / (m_hiCurve - m_loCurve));		// linear membership fct
	bPCurveX	= Input[PCURV] >= 0.0 ? 1.0 - bPCurve : 0.0;
	bPCurveV	= Input[PCURV] <= 0.0 ? 1.0 - bPCurve : 0.0;

	// membership tcurv straight
	bTCurve		= fabs(Input[TCURV]) <= m_loCurve ? 1.0 : (fabs(Input[TCURV]) >= m_hiCurve ? 0.0
				: (m_hiCurve - fabs(Input[TCURV])) / (m_hiCurve - m_loCurve));		// linear membership fct
	bTCurveX	= Input[TCURV] >= 0.0 ? 1.0 - bTCurve : 0.0;
	bTCurveV	= Input[TCURV] <= 0.0 ? 1.0 - bTCurve : 0.0;

	// membership maxcurv straight
	bMaxCurve	= fabs(Input[MAXCURV]) <= m_loCurve ? 1.0 : (fabs(Input[MAXCURV]) >= m_hiCurve ? 0.0
				: (m_hiCurve - fabs(Input[MAXCURV])) / (m_hiCurve - m_loCurve));	// linear membership fct
	bMaxCurveX	= Input[MAXCURV] >= 0.0 ? 1.0 - bMaxCurve : 0.0;
	bMaxCurveV	= Input[MAXCURV] <= 0.0 ? 1.0 - bMaxCurve : 0.0;

	// membership mincurv straight
	bMinCurve	= fabs(Input[MINCURV]) <= m_loCurve ? 1.0 : (fabs(Input[MINCURV]) >= m_hiCurve ? 0.0
				: (m_hiCurve - fabs(Input[MINCURV])) / (m_hiCurve - m_loCurve));	// linear membership fct
	bMinCurveX	= Input[MINCURV] >= 0.0 ? 1.0 - bMinCurve : 0.0;
	bMinCurveV	= Input[MINCURV] <= 0.0 ? 1.0 - bMinCurve : 0.0;

	//-----------------------------------------------------
	M[PLAIN]	= GET_MINIMUM(bFlat, bMaxCurve, bMinCurve);
	Element		= Form_Def[PLAIN].Value;
	MaxMem		= M[PLAIN];

	M[PIT]		= GET_MINIMUM(bFlat, bMaxCurveV, bMinCurveV);
	if( M[PIT] > MaxMem ) 
	{ 
		Element	= Form_Def[PIT].Value;
		MaxMem	= M[PIT]; 
		MaxMem2	= M[PLAIN];
	}
	else
	{
		MaxMem2	= M[PIT];
	}

	M[PEAK]		= GET_MINIMUM(bFlat, bMaxCurveX, bMinCurveX);
	if( M[PEAK] > MaxMem )
	{
		Element	= Form_Def[PEAK].Value;
		MaxMem	= M[PEAK];
	}
	else if( M[PEAK] > MaxMem2 )
	{
		MaxMem2	= M[PEAK];
	}

	M[RIDGE]	= GET_MINIMUM(bFlat, bMaxCurveX, bMinCurve);
	if( M[RIDGE] > MaxMem )
	{ 
		Element	= Form_Def[RIDGE].Value;
		MaxMem	= M[RIDGE];
	}
	else if( M[RIDGE] > MaxMem2 )
	{
		MaxMem2	= M[RIDGE];
	}

	M[CHANNEL]   = GET_MINIMUM(bFlat, bMaxCurve, bMinCurveV);
	if( M[CHANNEL] > MaxMem )
	{ 
		Element	= Form_Def[CHANNEL].Value;
		MaxMem	= M[CHANNEL];
	}
	else if( M[CHANNEL] > MaxMem2 )
	{
		MaxMem2	= M[CHANNEL];
	}

	M[SADDLE]   = GET_MINIMUM(bFlat, bMaxCurveX, bMinCurveV);
	if( M[SADDLE] > MaxMem )
	{ 
		Element	= Form_Def[SADDLE].Value;
		MaxMem	= M[SADDLE];
	}
	else if( M[SADDLE] > MaxMem2 )
	{
		MaxMem2	= M[SADDLE];
	}

	M[BSLOPE]   = GET_MINIMUM(bSlope, bPCurve, bTCurve);
	if( M[BSLOPE] > MaxMem )
	{ 
		Element	= Form_Def[BSLOPE].Value;
		MaxMem	= M[BSLOPE];
	}
	else if( M[BSLOPE] > MaxMem2 )
	{
		MaxMem2	= M[BSLOPE];
	}

	M[FSLOPE]   = GET_MINIMUM(bSlope, bPCurveV, bTCurve);
	if( M[FSLOPE] > MaxMem )
	{ 
		Element	= Form_Def[FSLOPE].Value;
		MaxMem	= M[FSLOPE];
	}
	else if( M[FSLOPE] > MaxMem2 )
	{
		MaxMem2	= M[FSLOPE];
	}

	M[SSLOPE]   = GET_MINIMUM(bSlope, bPCurveX, bTCurve);
	if( M[SSLOPE] > MaxMem )
	{ 
		Element	= Form_Def[SSLOPE].Value;
		MaxMem	= M[SSLOPE];
	}
	else if( M[SSLOPE] > MaxMem2 )
	{
		MaxMem2	= M[SSLOPE];
	}

	M[HOLLOW]   = GET_MINIMUM(bSlope, bPCurve, bTCurveV);
	if( M[HOLLOW] > MaxMem )
	{ 
		Element	= Form_Def[HOLLOW].Value;
		MaxMem	= M[HOLLOW];
	}
	else if( M[HOLLOW] > MaxMem2 )
	{
		MaxMem2	= M[HOLLOW];
	}

	M[FHOLLOW]   = GET_MINIMUM(bSlope, bPCurveV, bTCurveV);
	if( M[FHOLLOW] > MaxMem )
	{ 
		Element	= Form_Def[FHOLLOW].Value;
		MaxMem	= M[FHOLLOW];
	}
	else if( M[FHOLLOW] > MaxMem2 )
	{
		MaxMem2	= M[FHOLLOW];
	}

	M[SHOLLOW]   = GET_MINIMUM(bSlope, bPCurveX, bTCurveV);
	if( M[SHOLLOW] > MaxMem )
	{ 
		Element	= Form_Def[SHOLLOW].Value;
		MaxMem	= M[SHOLLOW];
	}
	else if( M[SHOLLOW] > MaxMem2 )
	{
		MaxMem2	= M[SHOLLOW];
	}

	M[SPUR]   = GET_MINIMUM(bSlope, bPCurve, bTCurveX);
	if( M[SPUR] > MaxMem )
	{ 
		Element	= Form_Def[SPUR].Value;
		MaxMem	= M[SPUR];
	}
	else if( M[SPUR] > MaxMem2 )
	{
		MaxMem2	= M[SPUR];
	}

	M[FSPUR] = GET_MINIMUM(bSlope, bPCurveV, bTCurveX);
	if( M[FSPUR] > MaxMem )
	{ 
		Element	= Form_Def[FSPUR].Value;
		MaxMem	= M[FSPUR];
	}
	else if( M[FSPUR] > MaxMem2 )
	{
		MaxMem2	= M[FSPUR];
	}

	M[SSPUR] = GET_MINIMUM(bSlope, bPCurveX, bTCurveX);
	if( M[SSPUR] > MaxMem )
	{ 
		Element	= Form_Def[SSPUR].Value;
		MaxMem	= M[SSPUR];
	}
	else if( M[SSPUR] > MaxMem2 )
	{
		MaxMem2	= M[SSPUR];
	}

	//-----------------------------------------------------
	double	summem	= 0.0;	// = %PLAIN% + %PIT% + %PEAK% + %RIDGE% + %CHANNEL% + %SADDLE% + %BSLOPE% + %FSLOPE% + %SSLOPE% + %HOLLOW% + %FHOLLOW% + %SHOLLOW% + %SPUR% + %FSPUR% + %SSPUR%

	for(int i=0; i<FE_COUNT; i++)
	{
		summem	+= M[i];
	}

	// entropy: / ln (15 elements)
	Entropy	= -0.3692693 *
		( (M[PLAIN]   <= 0 || summem <= 0 ? 0 : M[PLAIN]   * log(M[PLAIN]  ) / summem)
		+ (M[PIT]     <= 0 || summem <= 0 ? 0 : M[PIT]     * log(M[PIT]    ) / summem)
		+ (M[PEAK]    <= 0 || summem <= 0 ? 0 : M[PEAK]    * log(M[PEAK]   ) / summem)
		+ (M[RIDGE]   <= 0 || summem <= 0 ? 0 : M[RIDGE]   * log(M[RIDGE]  ) / summem)
		+ (M[CHANNEL] <= 0 || summem <= 0 ? 0 : M[CHANNEL] * log(M[CHANNEL]) / summem)
		+ (M[SADDLE]  <= 0 || summem <= 0 ? 0 : M[SADDLE]  * log(M[SADDLE] ) / summem)
		+ (M[BSLOPE]  <= 0 || summem <= 0 ? 0 : M[BSLOPE]  * log(M[BSLOPE] ) / summem)
		+ (M[FSLOPE]  <= 0 || summem <= 0 ? 0 : M[FSLOPE]  * log(M[FSLOPE] ) / summem)
		+ (M[SSLOPE]  <= 0 || summem <= 0 ? 0 : M[SSLOPE]  * log(M[SSLOPE] ) / summem)
		+ (M[HOLLOW]  <= 0 || summem <= 0 ? 0 : M[HOLLOW]  * log(M[HOLLOW] ) / summem)
		+ (M[FHOLLOW] <= 0 || summem <= 0 ? 0 : M[FHOLLOW] * log(M[FHOLLOW]) / summem)
		+ (M[SHOLLOW] <= 0 || summem <= 0 ? 0 : M[SHOLLOW] * log(M[SHOLLOW]) / summem)
		+ (M[SPUR]    <= 0 || summem <= 0 ? 0 : M[SPUR]    * log(M[SPUR]   ) / summem)
		+ (M[FSPUR]   <= 0 || summem <= 0 ? 0 : M[FSPUR]   * log(M[FSPUR]  ) / summem)
		+ (M[SSPUR]   <= 0 || summem <= 0 ? 0 : M[SSPUR]   * log(M[SSPUR]  ) / summem)
		- (summem <= 0 ? 0 : log(summem))
	);

	CI	= MaxMem2 / MaxMem;	// confusion index

	//-----------------------------------------------------
	return( true );
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
