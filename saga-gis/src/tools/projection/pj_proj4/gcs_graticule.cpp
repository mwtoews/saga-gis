
///////////////////////////////////////////////////////////
//                                                       //
//                         SAGA                          //
//                                                       //
//      System for Automated Geoscientific Analyses      //
//                                                       //
//                     Tool Library                      //
//                   Projection_Proj4                    //
//                                                       //
//-------------------------------------------------------//
//                                                       //
//                   gcs_graticule.cpp                   //
//                                                       //
//                 Copyright (C) 2014 by                 //
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
#include "gcs_graticule.h"


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#define AXIS_LEFT	1
#define AXIS_RIGHT	2
#define AXIS_BOTTOM	3
#define AXIS_TOP	4

//---------------------------------------------------------
enum
{
	DEG_PREC_AUTO,
	DEG_PREC_FULL,
	DEG_PREC_SEC,
	DEG_PREC_MIN,
	DEG_PREC_DEG
};


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
CGCS_Graticule::CGCS_Graticule(void)
{
	Set_Name		(_TL("Latitude/Longitude Graticule"));

	Set_Author		("O.Conrad (c) 2014");

	Set_Description	(_TW(
		"Creates a longitude/latitude graticule for the extent and projection of the input shapes layer. "
	));

	Set_Description	(Get_Description() + "\n" + CSG_CRSProjector::Get_Description());

	//-----------------------------------------------------
	Parameters.Add_Shapes("",
		"GRATICULE"	, _TL("Graticule"),
		_TL(""),
		PARAMETER_OUTPUT, SHAPE_TYPE_Line
	);

	Parameters.Add_Shapes("",
		"COORDS"	, _TL("Frame Coordinates"),
		_TL(""),
		PARAMETER_OUTPUT_OPTIONAL, SHAPE_TYPE_Point
	);

	Parameters.Add_Node("",
		"NODE_GRID"	, _TL("Graticule"),
		_TL("")
	);

	Parameters.Add_Node("NODE_GRID", "NODE_X", _TL("X Range"), _TL(""));
	Parameters.Add_Double("NODE_X" , "XMIN"  , _TL("Minimum"), _TL(""));
	Parameters.Add_Double("NODE_X" , "XMAX"  , _TL("Maximum"), _TL(""));

	Parameters.Add_Node("NODE_GRID", "NODE_Y", _TL("Y Range"), _TL(""));
	Parameters.Add_Double("NODE_Y" , "YMIN"  , _TL("Minimum"), _TL(""));
	Parameters.Add_Double("NODE_Y" , "YMAX"  , _TL("Maximum"), _TL(""));

	Parameters.Add_Choice("NODE_GRID",
		"INTERVAL"	, _TL("Interval"),
		_TL(""),
		CSG_String::Format("%s|%s",
			_TL("fixed interval"),
			_TL("fitted interval")
		), 0
	);

	Parameters.Add_Double("NODE_GRID",
		"FIXED"		, _TL("Fixed Interval (Degree)"),
		_TL(""),
		1., 0., true, 20.
	);

	Parameters.Add_Int("NODE_GRID",
		"FITTED"	, _TL("Number of Intervals"),
		_TL(""),
		10, 1, true
	);

	Parameters.Add_Double("NODE_GRID",
		"RESOLUTION", _TL("Minimum Resolution (Degree)"),
		_TL(""),
		0.5, 0., true
	);
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
int CGCS_Graticule::On_Parameter_Changed(CSG_Parameters *pParameters, CSG_Parameter *pParameter)
{
	if( pParameter->Cmp_Identifier("CRS_PICKER") )
	{
		CSG_Rect Extent;

		if(	pParameter->asParameters()->Get_Parameter("CRS_GRID"  )->asGrid() )
		{
			Extent = pParameter->asParameters()->Get_Parameter("CRS_GRID"  )->asGrid  ()->Get_Extent();
		}

		if(	pParameter->asParameters()->Get_Parameter("CRS_SHAPES")->asShapes() )
		{
			Extent = pParameter->asParameters()->Get_Parameter("CRS_SHAPES")->asShapes()->Get_Extent();
		}

		if( Extent.Get_XRange() > 0. && Extent.Get_YRange() > 0. )
		{
			pParameters->Set_Parameter("XMIN", Extent.Get_XMin());
			pParameters->Set_Parameter("XMAX", Extent.Get_XMax());
			pParameters->Set_Parameter("YMIN", Extent.Get_YMin());
			pParameters->Set_Parameter("YMAX", Extent.Get_YMax());
		}
	}

	return( CCRS_Base::On_Parameter_Changed(pParameters, pParameter) );
}

//---------------------------------------------------------
int CGCS_Graticule::On_Parameters_Enable(CSG_Parameters *pParameters, CSG_Parameter *pParameter)
{
	if(	pParameter->Cmp_Identifier("INTERVAL") )
	{
		pParameters->Set_Enabled("FIXED" , pParameter->asInt() == 0);
		pParameters->Set_Enabled("FITTED", pParameter->asInt() == 1);
	}

	return( CCRS_Base::On_Parameters_Enable(pParameters, pParameter) );
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CGCS_Graticule::On_Execute(void)
{
	CSG_Projection Projection;

	if( !Get_Projection(Projection) || !m_Projector.Set_Transformation(CSG_Projection::Get_GCS_WGS84(), Projection) )
	{
		m_Projector.Destroy();

		return( false );
	}

	//-----------------------------------------------------
	CSG_Rect Extent(
		Parameters("XMIN")->asDouble(),
		Parameters("YMIN")->asDouble(),
		Parameters("XMAX")->asDouble(),
		Parameters("YMAX")->asDouble()
	);

	if( !Get_Graticule(Extent) )
	{
		m_Projector.Destroy();

		return( false );
	}

	//-----------------------------------------------------
	m_Projector.Destroy();

	return( true );
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CGCS_Graticule::Get_Graticule(const CSG_Rect &Extent)
{
	CSG_Rect r; double Interval;

	if( !Get_Extent(Extent, r) || (Interval = Get_Interval(r)) <= 0. )
	{
		return( false );
	}

	//-----------------------------------------------------
	r.xMin = Interval * floor(r.Get_XMin() / Interval);
	r.xMax = Interval * ceil (r.Get_XMax() / Interval);
	r.yMin = Interval * floor(r.Get_YMin() / Interval);
	r.yMax = Interval * ceil (r.Get_YMax() / Interval);

	bool bClip = false;

	r.Inflate(Interval, false);

	if( r.Get_XMin() < -180. ) r.xMin = -180.;
	if( r.Get_XMax() >  180. ) r.xMax =  180.;
	if( r.Get_YMin() <  -90. ) r.yMin =  -90.;
	if( r.Get_YMax() >   90. ) r.yMax =   90.;

	//-----------------------------------------------------
	double Resolution = Parameters("RESOLUTION")->asDouble(); if( Resolution <= 0. ) Resolution = Interval;

	if( Interval > Resolution )
	{
		Resolution = Interval / ceil(Interval / Resolution);
	}

	//-----------------------------------------------------
	CSG_Shapes *pGraticule = Parameters("GRATICULE")->asShapes();

	pGraticule->Create(SHAPE_TYPE_Line);
	pGraticule->Set_Name(_TL("Graticule"));

	pGraticule->Add_Field("TYPE"  , SG_DATATYPE_String);
	pGraticule->Add_Field("LABEL" , SG_DATATYPE_String);
	pGraticule->Add_Field("DEGREE", SG_DATATYPE_Double);

	//-----------------------------------------------------
	CSG_Shapes *pCoordinates = Parameters("COORDS")->asShapes();

	if( pCoordinates )
	{
		pCoordinates->Create(SHAPE_TYPE_Point);
		pCoordinates->Set_Name(_TL("Coordinates"));

		pCoordinates->Add_Field("TYPE" , SG_DATATYPE_String);
		pCoordinates->Add_Field("LABEL", SG_DATATYPE_String);
	}

	//-----------------------------------------------------
	CSG_Shapes Clip(SHAPE_TYPE_Polygon);
	CSG_Shape *pClip = Clip.Add_Shape();

	pClip->Add_Point(Extent.Get_XMin(), Extent.Get_YMin());
	pClip->Add_Point(Extent.Get_XMin(), Extent.Get_YMax());
	pClip->Add_Point(Extent.Get_XMax(), Extent.Get_YMax());
	pClip->Add_Point(Extent.Get_XMax(), Extent.Get_YMin());
	pClip->Add_Point(Extent.Get_XMin(), Extent.Get_YMin());

	//-----------------------------------------------------
	for(double y=r.Get_YMin(); y<=r.Get_YMax(); y+=Interval)
	{
		CSG_Shape *pLine = pGraticule->Add_Shape();

		pLine->Set_Value(0, "LAT");
		pLine->Set_Value(1, Get_Degree(y, DEG_PREC_DEG));
		pLine->Set_Value(2, y);

		for(double x=r.Get_XMin(); x<=r.Get_XMax(); x+=Interval)
		{
			CSG_Point p(x, y); m_Projector.Get_Projection(p); pLine->Add_Point(p);

			if( Resolution < Interval && x < r.Get_XMax() )
			{
				for(double i=x+Resolution; i<x+Interval; i+=Resolution)
				{
					CSG_Point p(i, y); m_Projector.Get_Projection(p); pLine->Add_Point(p);
				}
			}
		}

		Get_Coordinate(Extent, pCoordinates, pLine, AXIS_LEFT);
		Get_Coordinate(Extent, pCoordinates, pLine, AXIS_RIGHT);

		if( bClip && !SG_Shape_Get_Intersection(pLine, pClip->asPolygon()) )
		{
			pGraticule->Del_Shape(pLine);
		}
	}

	//-----------------------------------------------------
	for(double x=r.Get_XMin(); x<=r.Get_XMax(); x+=Interval)
	{
		CSG_Shape *pLine = pGraticule->Add_Shape();

		pLine->Set_Value(0, "LON");
		pLine->Set_Value(1, Get_Degree(x, DEG_PREC_DEG));
		pLine->Set_Value(2, x);

		for(double y=r.Get_YMin(); y<=r.Get_YMax(); y+=Interval)
		{
			CSG_Point p(x, y); m_Projector.Get_Projection(p); pLine->Add_Point(p);

			if( Resolution < Interval && y < r.Get_YMax() )
			{
				for(double i=y+Resolution; i<y+Interval; i+=Resolution)
				{
					CSG_Point p(x, i); m_Projector.Get_Projection(p); pLine->Add_Point(p);
				}
			}
		}

		Get_Coordinate(Extent, pCoordinates, pLine, AXIS_BOTTOM);
		Get_Coordinate(Extent, pCoordinates, pLine, AXIS_TOP);

		if( bClip && !SG_Shape_Get_Intersection(pLine, pClip->asPolygon()) )
		{
			pGraticule->Del_Shape(pLine);
		}
	}

	//-----------------------------------------------------
	return( true );
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CGCS_Graticule::Get_Coordinate(const CSG_Rect &Extent, CSG_Shapes *pCoordinates, CSG_Shape *pLine, int Axis)
{
	if( !pCoordinates || !Extent.Intersects(pLine->Get_Extent()) || pLine->Get_Point_Count(0) < 2 )
	{
		return( false );
	}

	TSG_Point	A[2], B[2], C;

	switch( Axis )
	{
	case AXIS_LEFT  : A[0].x = A[1].x = Extent.Get_XMin(); A[0].y = Extent.Get_YMin(); A[1].y = Extent.Get_YMax(); break;
	case AXIS_RIGHT : A[0].x = A[1].x = Extent.Get_XMax(); A[0].y = Extent.Get_YMin(); A[1].y = Extent.Get_YMax(); break;
	case AXIS_BOTTOM: A[0].y = A[1].y = Extent.Get_YMin(); A[0].x = Extent.Get_XMin(); A[1].x = Extent.Get_XMax(); break;
	case AXIS_TOP   : A[0].y = A[1].y = Extent.Get_YMax(); A[0].x = Extent.Get_XMin(); A[1].x = Extent.Get_XMax(); break;

	default:
		return( false );
	}

	//-----------------------------------------------------
	B[1]	= pLine->Get_Point();

	for(int i=1; i<pLine->Get_Point_Count(); i++)
	{
		B[0]	= B[1];
		B[1]	= pLine->Get_Point(i);

		if( SG_Get_Crossing(C, A[0], A[1], B[0], B[1], true) )
		{
			CSG_Shape	*pPoint	= pCoordinates->Add_Shape();
			pPoint->Add_Point(C);
			pPoint->Set_Value(0, CSG_String(pLine->asString(0)) + (Axis == AXIS_LEFT || Axis == AXIS_BOTTOM ? "_MIN" : "_MAX"));
			pPoint->Set_Value(1, pLine->asString(1));

			return( true );
		}
	}

	//-----------------------------------------------------
	switch( Axis )
	{
	case AXIS_LEFT  : C	= pLine->Get_Point(0, 0, true ); break;
	case AXIS_RIGHT : C	= pLine->Get_Point(0, 0, false); break;
	case AXIS_BOTTOM: C	= pLine->Get_Point(0, 0, true ); break;
	case AXIS_TOP   : C	= pLine->Get_Point(0, 0, false); break;
	}

	if( Extent.Contains(C) )
	{
		CSG_Shape	*pPoint	= pCoordinates->Add_Shape();
		pPoint->Add_Point(C);
		pPoint->Set_Value(0, CSG_String(pLine->asString(0)) + (Axis == AXIS_LEFT || Axis == AXIS_BOTTOM ? "_MIN" : "_MAX"));
		pPoint->Set_Value(1, pLine->asString(1));

		return( true );
	}

	//-----------------------------------------------------
	return( false );
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
double CGCS_Graticule::Get_Interval(const CSG_Rect &Extent)
{
	if( Parameters("INTERVAL")->asInt() == 0 )
	{
		return( Parameters("FIXED")->asDouble() );
	}

	double	Interval	= Extent.Get_XRange() > Extent.Get_YRange() ? Extent.Get_XRange() : Extent.Get_YRange();

	if( Interval > 360. )
	{
		Interval	= 360.;
	}

	Interval	= Interval / Parameters("FITTED")->asInt();

	double	d	= pow(10., (int)(log10(Interval)) - (Interval < 1. ? 1. : 0.));

	Interval	= (int)(Interval / d) * d;

	return( Interval );
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CGCS_Graticule::Get_Extent(const CSG_Rect &Extent, CSG_Rect &r)
{
	if( m_Projector.Set_Inverse() )
	{
		double		x, y, d;

		CSG_Point	p(Extent.Get_XMin(), Extent.Get_YMin());
		
		m_Projector.Get_Projection(p);	r.Assign(p, p);

		d	= Extent.Get_XRange() / 10.;

		for(y=Extent.Get_YMin(), x=Extent.Get_XMin(); x<=Extent.Get_XMax(); x+=d)
		{
			p.Assign(x, y);	m_Projector.Get_Projection(p);	r.Union(p);
		}

		for(y=Extent.Get_YMax(), x=Extent.Get_XMin(); x<=Extent.Get_XMax(); x+=d)
		{
			p.Assign(x, y);	m_Projector.Get_Projection(p);	r.Union(p);
		}

		d	= Extent.Get_YRange() / 10.;

		for(x=Extent.Get_XMin(), y=Extent.Get_YMin(); y<=Extent.Get_YMax(); y+=d)
		{
			p.Assign(x, y);	m_Projector.Get_Projection(p);	r.Union(p);
		}

		for(x=Extent.Get_XMax(), y=Extent.Get_YMin(); y<=Extent.Get_YMax(); y+=d)
		{
			p.Assign(x, y);	m_Projector.Get_Projection(p);	r.Union(p);
		}

		m_Projector.Set_Inverse(false);

		if( r.Get_XMin() < -180. ) r.xMin = -180.; else if( r.Get_XMax() > 180. ) r.xMax = 180.;
		if( r.Get_YMin() <  -90. ) r.yMin =  -90.; else if( r.Get_YMax() >  90. ) r.yMax =  90.;

		return( r.Get_XRange() > 0. && r.Get_YRange() > 0. );
	}

	return( false );
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
CSG_String CGCS_Graticule::Get_Degree(double Value, int Precision)
{
	if( Precision == DEG_PREC_DEG )
	{
		return( SG_Get_String(Value, -12) + "\xb0" );
	}

	SG_Char		c;
	int			d, h;
	double		s;
	CSG_String	String;

	if( Value < 0. )
	{
		Value	= -Value;
		c		= '-';
	}
	else
	{
		c		= '+';
	}

	Value	= fmod(Value, 360.);
	d		= (int)Value;
	Value	= 60. * (Value - d);
	h		= (int)Value;
	Value	= 60. * (Value - h);
	s		= Value;

	if( s > 0. || Precision == DEG_PREC_FULL )
	{
		String.Printf("%c%d\xb0%02d'%02.*f''", c, d, h, SG_Get_Significant_Decimals(s), s);
	}
	else if( h > 0 || Precision == DEG_PREC_MIN )
	{
		String.Printf("%c%d\xb0%02d'"        , c, d, h);
	}
	else
	{
		String.Printf("%c%d\xb0"             , c, d);
	}

	return( String );
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
