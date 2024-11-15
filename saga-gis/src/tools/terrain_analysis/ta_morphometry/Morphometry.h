
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
//                     Morphometry.h                     //
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
#ifndef HEADER_INCLUDED__Morphometry_H
#define HEADER_INCLUDED__Morphometry_H


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#include <saga_api/saga_api.h>


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
class CMorphometry : public CSG_Tool_Grid
{
public:
	CMorphometry(void);


protected:

	virtual int				On_Parameters_Enable	(CSG_Parameters *pParameters, CSG_Parameter *pParameter);

	virtual bool			On_Execute				(void);


private:

	int						m_Unit_Slope, m_Unit_Aspect;

	CSG_Grid				*m_pDTM, *m_pSlope, *m_pAspect, *m_pNorthness, *m_pEastness, *m_pC_Gene, *m_pC_Prof, *m_pC_Plan, *m_pC_Tang, *m_pC_Long, *m_pC_Cros, *m_pC_Mini, *m_pC_Maxi, *m_pC_Tota, *m_pC_Roto;


	//-----------------------------------------------------
	void					Get_SubMatrix3x3		(int x, int y, double Z[ 9], int Orientation = 0);
	void					Get_SubMatrix5x5		(int x, int y, double Z[25], int Orientation = 0);

	//-----------------------------------------------------
	void					Set_NoData				(int x, int y);
	void					Set_Gradient			(int x, int y, double Slope, double Aspect);

	void					Set_From_Polynom		(int x, int y, double r, double t, double s, double p, double q);

	//-----------------------------------------------------
	void					Set_MaximumSlope		(int x, int y);
	void					Set_Tarboton			(int x, int y);
	void					Set_LeastSquare			(int x, int y);
	void					Set_Evans				(int x, int y);
	void					Set_Heerdegen			(int x, int y);
	void					Set_BRM					(int x, int y);
	void					Set_Zevenbergen			(int x, int y);
	void					Set_Haralick			(int x, int y);
	void					Set_Florinsky			(int x, int y);

};


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#endif // #ifndef HEADER_INCLUDED__Morphometry_H
