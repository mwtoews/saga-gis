
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
//                  crs_indicatrix.cpp                   //
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
#include "crs_indicatrix.h"


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
CCRS_Indicatrix::CCRS_Indicatrix(void)
{
	Set_Name		(_TL("Tissot's Indicatrix"));

	Set_Author		("O.Conrad (c) 2014");

	Set_Description	(_TW(
		"Creates a shapes layer with Tissot's indicatrices for chosen projection."
	));

	Set_Description	(Get_Description() + "\n" + CSG_CRSProjector::Get_Description());

	//-----------------------------------------------------
	Parameters.Add_Shapes("",
		"TARGET", _TL("Indicatrix"),
		_TL(""),
		PARAMETER_OUTPUT
	);

	Parameters.Add_Int("",
		"NY"    , _TL("Number in Latitudinal Direction"),
		_TL(""),
		5, 1, true
	);

	Parameters.Add_Int("",
		"NX"    , _TL("Number in Meridional Direction"),
		_TL(""),
		11, 1, true
	);

	Parameters.Add_Double("",
		"SCALE" , _TL("Size"),
		_TL(""),
		25., 1., true
	);
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
enum
{
	FIELD_LON	= 0,
	FIELD_LAT,
	FIELD_h,
	FIELD_k,
	FIELD_a,
	FIELD_b,
	FIELD_w,
	FIELD_PHI
};

//---------------------------------------------------------
bool CCRS_Indicatrix::On_Execute_Transformation(void)
{
	double yStep = 180. / Parameters("NY")->asDouble();
	double xStep = 360. / Parameters("NX")->asDouble();

	m_Size	= 1.;
	m_Scale	= (40000000. / 360.) * (yStep < xStep ? yStep : xStep) * 0.005 * Parameters("SCALE")->asDouble() / m_Size;

	//-----------------------------------------------------
	m_Circle.Add(0., 0.);
	m_Circle.Add(sin(M_PI_090), cos(M_PI_090));
	m_Circle.Add(0., 0.);

	for(double a=0.; a<M_PI_360; a+=2.*M_DEG_TO_RAD)
	{
		m_Circle.Add(sin(a), cos(a));
	}

	//-----------------------------------------------------
	CSG_Shapes *pTarget = Parameters("TARGET")->asShapes();

	pTarget->Create(SHAPE_TYPE_Polygon, CSG_String::Format("%s [%s]", _TL("Indicatrix"), m_Projector.Get_Target().Get_Name().c_str()));

	pTarget->Get_Projection() = m_Projector.Get_Target();

	pTarget->Add_Field("LON", SG_DATATYPE_Double);
	pTarget->Add_Field("LAT", SG_DATATYPE_Double);
	pTarget->Add_Field("h"  , SG_DATATYPE_Double);
	pTarget->Add_Field("k"  , SG_DATATYPE_Double);
	pTarget->Add_Field("a"  , SG_DATATYPE_Double);
	pTarget->Add_Field("b"  , SG_DATATYPE_Double);
	pTarget->Add_Field("w"  , SG_DATATYPE_Double);
	pTarget->Add_Field("PHI", SG_DATATYPE_Double);

	//-----------------------------------------------------
	int nDropped = 0;

	for(double lat=-90.+yStep/2.; lat<90.; lat+=yStep)
	{
		for(double lon=-180.+xStep/2.; lon<180.; lon+=xStep)
		{
			CSG_Shape *pIndicatrix = pTarget->Add_Shape();

			if( !Get_Indicatrix(lon, lat, pIndicatrix) )
			{
				nDropped++;

				pTarget->Del_Shape(pIndicatrix);
			}
		}
	}

	//-----------------------------------------------------
	if( nDropped > 0 )
	{
		Message_Fmt("\n%s: %d %s", pTarget->Get_Name(), nDropped, _TL("shapes have been dropped"));
	}

	m_Circle.Clear();

	return( pTarget->Get_Count() > 0 );
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CCRS_Indicatrix::Get_Indicatrix(double lon, double lat, CSG_Shape *pIndicatrix)
{
	const char *PROJ = "+proj=ortho +lon_0=%f +lat_0=%f +datum=WGS84";
	const char *WKT2 =
		"PROJCRS[\"custom\","
		"    BASEGEODCRS[\"custom\","
		"        DATUM[\"World Geodetic System 1984\","
		"            ELLIPSOID[\"WGS 84\",6378137,298.257223563],"
		"            ID[\"EPSG\",6326]],"
		"        UNIT[\"degree\",0.0174532925199433,"
		"            ID[\"EPSG\",9122]]],"
		"    CONVERSION[\"<custom>\","
		"        METHOD[\"Orthographic\","
		"            ID[\"EPSG\",9840]],"
		"        PARAMETER[\"Latitude of natural origin\",%f,"
		"            ID[\"EPSG\",8801]],"
		"        PARAMETER[\"Longitude of natural origin\",%f,"
		"            ID[\"EPSG\",8802]],"
		"        PARAMETER[\"False easting\",0,"
		"            ID[\"EPSG\",8806]],"
		"        PARAMETER[\"False northing\",0,"
		"            ID[\"EPSG\",8807]]],"
		"    CS[Cartesian,2],"
		"        AXIS[\"(E)\",east],"
		"        AXIS[\"(N)\",north],"
		"        UNIT[\"metre\",1,"
		"            ID[\"EPSG\",9001]]]";

	CSG_Projection Source(CSG_String::Format(WKT2, lon, lat), CSG_String::Format(PROJ, lon, lat));

	if( !m_Projector.Set_Source(Source, true) )
	{
		return( false );
	}

	CSG_Point Center;

	if( !m_Projector.Get_Projection(Center) )
	{
		return( false );
	}

	//-----------------------------------------------------
	CSG_Point Point(m_Size, 0.);

	if( !m_Projector.Get_Projection(Point) )
	{
		return( false );
	}

	double h = SG_Get_Distance(Center, Point) / m_Size;

	//-----------------------------------------------------
	Point.x = 0.; Point.y = m_Size;

	if( !m_Projector.Get_Projection(Point) )
	{
		return( false );
	}

	double k = SG_Get_Distance(Center, Point) / m_Size;

	//-----------------------------------------------------
	double a = h > k ? h : k; // major semi-axis
	double b = h > k ? k : h; // minor semi-axis

	for(int iPoint=0; iPoint<m_Circle.Get_Count(); iPoint++)
	{
		Point.x = m_Size * m_Circle[iPoint].x;
		Point.y = m_Size * m_Circle[iPoint].y;

		if( !m_Projector.Get_Projection(Point) )
		{
			return( false );
		}

		if( iPoint >= 3 )
		{
			double d = SG_Get_Distance(Center, Point) / m_Size;

			if( a < d )
			{
				a = d;
			}
			else if( b > d )
			{
				b = d;
			}
		}

		Point.x = Center.x + m_Scale * (Point.x - Center.x);
		Point.y = Center.y + m_Scale * (Point.y - Center.y);

		pIndicatrix->Add_Point(Point);
	}

	//-----------------------------------------------------
	double w = 2. * M_RAD_TO_DEG * asin((a - b) / (a + b));
	double phi = a * b;

	pIndicatrix->Set_Value(FIELD_LON, SG_Get_Rounded(lon, 2));
	pIndicatrix->Set_Value(FIELD_LAT, SG_Get_Rounded(lat, 2));
	pIndicatrix->Set_Value(FIELD_h  , SG_Get_Rounded(h  , 2));
	pIndicatrix->Set_Value(FIELD_k  , SG_Get_Rounded(k  , 2));
	pIndicatrix->Set_Value(FIELD_a  , SG_Get_Rounded(a  , 2));
	pIndicatrix->Set_Value(FIELD_b  , SG_Get_Rounded(b  , 2));
	pIndicatrix->Set_Value(FIELD_w  , SG_Get_Rounded(w  , 1));
	pIndicatrix->Set_Value(FIELD_PHI, SG_Get_Rounded(phi, 1));

	return( true );
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
