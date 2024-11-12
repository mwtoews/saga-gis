///////////////////////////////////////////////////////////
//                                                       //
//                         SAGA                          //
//                                                       //
//      System for Automated Geoscientific Analyses      //
//                                                       //
//                     Tool Library                      //
//                     grid_analysis                     //
//                                                       //
//-------------------------------------------------------//
//                                                       //
//               AccumulationFunctions.cpp               //
//                                                       //
//               Copyright (C) 2009-2022 by              //
//                    Volker Wichmann                    //
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
//    e-mail:     wichmann@laserdata                     //
//                                                       //
//    contact:    Volker Wichmann                        //
//                LASERDATA GmbH                         //
//                Management and analysis of             //
//                laserscanning data                     //
//                Innsbruck, Austria                     //
//                                                       //
///////////////////////////////////////////////////////////

//---------------------------------------------------------


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#include "Grid_Accumulation_Functions.h"


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//-----------------------------------------------------------
CGrid_Accumulation_Functions::CGrid_Accumulation_Functions(void)
{
	Set_Name		(_TL("Accumulation Functions"));

	Set_Author		("V. Wichmann (c) 2009-2022");

    Set_Version     ("1.1");

	Parameters.Set_Description(_TW(
		"The tool allows one to use different \"accumulation functions\" to, e.g., move material over a \"local "
		"drain direction\" (LDD) network. The LDD net is computed for the supplied surface by MFD "
		"and D8 flow-routing algorithms. It is possible to switch from MFD to D8 as soon as a "
		"flow threshold is exceeded.\n"
		"The input to each cell on the grid can be supplied from e.g. time series and the material "
		"can be moved over the net in several ways. All of these, except the \"accuflux\" operation, "
		"compute both the flux and the state for a given cell. For time series modelling (batch "
		"processing), the state of each cell at time t can be initialized with the previous state t - 1.\n"
		"The capacity, fraction, threshold and trigger operations compute the fluxes and cell states "
		"at time t + 1 according to cell-specific parameters that control the way the flux is computed.\n"
		"The capacity function limits the cell-to-cell flux by a (channel) capacity control; the fraction "
		"function transports only a given proportion of material from cell to cell, the threshold "
		"function transports material only once a given threshold has been exceeded, and the trigger "
		"function transports nothing until a trigger value has been exceeded (at which point all "
		"accumulated material in the state of the cell is discharged to its downstream neighbour(s)).\n\n"
		"The following operations are supported:\n\n"
		"\t* ACCUFLUX: The accuflux function computes the new state of the attributes for the cell "
		"as the sum of the input cell values plus the cumulative sum of all upstream elements "
		"draining through the cell.\n\n"
		"\t* ACCUCAPACITYFLUX / STATE: The operation modifies the accumulation of flow over the "
		"network by a limiting transport capacity given in absolute values.\n\n"
		"\t* ACCUFRACTIONFLUX / STATE: The operation limits the flow over the network by a "
		"parameter which controls the proportion (0-1) of the material that can flow through each cell.\n\n"
		"\t* ACCUTHRESHOLDFLUX / STATE: The operation modifies the accumulation of flow over "
		"the network by limiting transport to values greater than a minimum threshold value "
		"per cell. No flow occurs if the threshold is not exceeded.\n\n"
		"\t* ACCUTRIGGERFLUX / STATE: The operation only allows transport (flux) to occur if "
		"a trigger value is exceeded, otherwise no transport occurs and storage accumulates.\n\n"
        "Instead of choosing a single global operation with the \"Operation\" choice parameter, an input "
        "grid can be provided which encodes the operation per grid cell. This makes it possible to use different "
        "operations across the LDD (e.g. for different land use classes). The cell values used to "
        "encode the operation in the grid are the index numbers of the \"Operation\" choice parameter:\n\n"
        "0: accuflux\n"
        "1: accucapacityflux / state\n"
        "2: accufractionflux / state\n"
        "3: accuthresholdflux / state\n"
        "4: accutriggerflux / state\n\n"
	));

	Add_Reference("Burrough, P.A.", "1998",
		"Dynamic Modelling and Geocomputation",
		"In: Longley, P.A., Brooks, S.M., McDonnell, R. & B. MacMillan [Eds.]: Geocomputation: A Primer. John Wiley & Sons, pp. 165-191."
	);
	
	Parameters.Add_Grid("",
		"SURFACE"	, _TL("Surface"), 
		_TL("Surface used to derive the LDD network, e.g. a DTM."), 
		PARAMETER_INPUT
	);

	Parameters.Add_Grid("",
		"INPUT"		, _TL("Input"), 
		_TL("Grid with the input values to accumulate."), 
		PARAMETER_INPUT
	);

	Parameters.Add_Grid("",
		"STATE_IN"	, _TL("State t"), 
		_TL("Grid describing the state of each cell at timestep t."), 
		PARAMETER_INPUT_OPTIONAL
	);

    Parameters.Add_Grid("",
        "OPERATION_GRID", _TL("Operation Grid"), 
        _TL("Grid encoding the mode of operation per grid cell. Can be used instead of a global setting (\"Operation\" choice). Operations use the same identifiers as the \"Operation\" choice parameter [0-4]."), 
        PARAMETER_INPUT_OPTIONAL
    );

	Parameters.Add_Grid("",
		"CONTROL"	, _TL("Operation Control"), 
		_TL("Depending on the mode of operation either transport capacity, transport fraction, threshold value or trigger value."), 
		PARAMETER_INPUT_OPTIONAL
	);

	Parameters.Add_Grid("LINEAR",
		"CTRL_LINEAR", _TL("Linear Flow Control Grid"), 
		_TL("The values of this grid are checked against the linear flow threshold to decide on the flow-routing algorithm."), 
		PARAMETER_INPUT_OPTIONAL
	);

	Parameters.Add_Grid("",
		"FLUX"		, _TL("Flux"), 
		_TL("Flux out of each cell, i.e. everything accumulated so far."), 
		PARAMETER_OUTPUT
	);

	Parameters.Add_Grid("",
		"STATE_OUT"	, _TL("State t + 1"), 
		_TL("Grid describing the state of each cell at timestep t + 1."), 
		PARAMETER_OUTPUT_OPTIONAL
	);

	Parameters.Add_Choice("",
		"OPERATION"	, _TL("Operation"),
		_TL("Select a mode of operation."),
		CSG_String::Format("%s|%s|%s|%s|%s",
			_TL("accuflux"),
			_TL("accucapacityflux / state"),
			_TL("accufractionflux / state"),
			_TL("accuthresholdflux / state"),
			_TL("accutriggerflux / state")
		),	0
	);

	Parameters.Add_Bool("",
		"LINEAR"	, _TL("Switch to Linear Flow"), 
		_TL("Switch from MFD8 to D8 if the linear flow threshold is crossed."), 
		true
	);

	Parameters.Add_Double("LINEAR",
		"THRES_LINEAR", _TL("Threshold Linear Flow"), 
		_TL("Threshold for linear flow, if exceeded D8 is used."), 
		0.0, 0.0, true
	);
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
int CGrid_Accumulation_Functions::On_Parameters_Enable(CSG_Parameters *pParameters, CSG_Parameter *pParameter)
{
    if(	pParameter->Cmp_Identifier("OPERATION_GRID") )
    {
        if (pParameter->asGrid() != NULL)
        {
            pParameters->Set_Enabled("CONTROL"      , true);
            pParameters->Set_Enabled("OPERATION"    , false);
        }
        else
        {
            pParameters->Set_Enabled("OPERATION"    , true);
            pParameters->Set_Enabled("CONTROL"      , pParameters->Get_Parameter("OPERATION")->asInt() > 0);
        }
    }

	if(	pParameter->Cmp_Identifier("OPERATION") )
	{
		pParameters->Set_Enabled("CONTROL", pParameter->asInt() > 0);
	}

	if(	pParameter->Cmp_Identifier(SG_T("LINEAR")) )
	{
		pParameters->Set_Enabled("THRES_LINEAR", pParameter->asBool());
		pParameters->Set_Enabled("CTRL_LINEAR" , pParameter->asBool());
	}

	return( CSG_Tool_Grid::On_Parameters_Enable(pParameters, pParameter) );
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CGrid_Accumulation_Functions::On_Execute(void)
{
	CSG_Grid		*pSurface, *pInput, *pFlux, *pControl, *pStateIn, *pStateOut, *pLinearCtrl, *pOperation;

	int				x, y, ix, iy;
	double			z, d, dz[8], dzSum, gradient, maxGrad, thresLinear;
	const double	Convergence	= 1.1;		// convergence factor MFD after FREEMAN (1991)
	int				steepestN;
	int				operation;
	bool			bLinearFlow;
	double			flux, d_flux, control, state, linearCtrl;


	pSurface		= Parameters("SURFACE")->asGrid();
	pInput			= Parameters("INPUT")->asGrid();
	pStateIn		= Parameters("STATE_IN")->asGrid();
	pStateOut		= Parameters("STATE_OUT")->asGrid();
    pOperation      = Parameters("OPERATION_GRID")->asGrid();
	pControl		= Parameters("CONTROL")->asGrid();
	pFlux			= Parameters("FLUX")->asGrid();
	pLinearCtrl		= Parameters("CTRL_LINEAR")->asGrid();

	operation		= Parameters("OPERATION")->asInt();

	bLinearFlow		= Parameters("LINEAR")->asBool();
	thresLinear		= Parameters("THRES_LINEAR")->asDouble();


	if( ((pOperation == NULL && operation != 0) || pOperation != NULL) && pControl == NULL)
	{
        Error_Set(_TL("You must specify an operation control grid as input!\n"));
		return( false );
	}

	if( pStateIn == NULL )
		pFlux->Assign(0.0);
	else
		pFlux->Assign(pStateIn);


	if( (pOperation != NULL || (pOperation == NULL && operation != 0)) && pStateOut == NULL )
	{
		pStateOut	= SG_Create_Grid(pInput, SG_DATATYPE_Double);
		Parameters("STATE_OUT")->Set_Value(pStateOut);
		pStateOut->Set_Name(SG_T("State (t)"));
	}


	if( !pSurface->Set_Index() )
	{
		Error_Set(_TL("index creation failed"));

		return( false );
	}

	for(sLong n=0; n<Get_NCells() && Set_Progress_Cells(n); n++)
	{
		if( pSurface->Get_Sorted(n, x, y, true, false) )
        {
		    if( pSurface->is_NoData(x, y) || pInput->is_NoData(x, y) || (operation != 0 && pControl->is_NoData(x, y)) || (pOperation != NULL && pOperation->is_NoData(x, y)) )
		    {
			    pFlux->Set_NoData(x, y);

			    if( pStateOut != NULL )
                {
				    pStateOut->Set_NoData(x, y);
                }
		    }
		    else
		    {
			    if( (pOperation != NULL || (pOperation == NULL && operation != 0)) )
                {
				    control		= pControl->asDouble(x, y);
                }

                if( pOperation != NULL )
                {
                    operation   = pOperation->asInt(x, y);
                }

			    flux			= pInput->asDouble(x, y) + pFlux->asDouble(x, y);
			
			    switch(operation)
		        {
			    case 0:			// accuflux
                    state	= 0.0;
				    break;

                case 1:			// accucapacityflux/state
                    if( flux > control )
				    {
					    state	= flux - control;
					    flux	= control;
				    }
				    else
					    state	= 0.0;
				    break;

                case 2:			// accufractionflux/state
				    state		= flux * (1.0 - control);
				    flux		*= control;
				    break;

			    case 3:			// accuthresholdflux/state
				    if( flux > control )
				    {
					    state	= control;
					    flux	-= control;
				    }
				    else
				    {
					    state	= flux;
					    flux	= 0.0;
				    }
				    break;

			    case 4:			// accutriggerflux/state
				    if( flux > control )
					    state	= 0.0;
				    else
				    {
					    state	= flux;
					    flux	= 0.0;
				    }
				    break;

                default:
                    state	= 0.0;
                    Message_Add(CSG_String::Format(_TL("Unknown operation value %d in grid cell %d,%d (x,y), computing accuflux ..."), operation, x, y));
                    break;
			    }

			    z				= pSurface->asDouble(x, y);
			    dzSum			= 0.0;
			    maxGrad			= 0.0;
			    bool bBorder	= false;

			    // get successor cells
			    //-----------------------------------------------------
			    if( pLinearCtrl != NULL )
				    linearCtrl = pLinearCtrl->asDouble(x, y);
			    else
				    linearCtrl = flux;

			    for(int i=0; i<8; i++)											
			    {
				    ix	= Get_xTo(i, x);			
				    iy	= Get_yTo(i, y);

				    if( is_InGrid(ix, iy) )
				    {
					    if( !pSurface->is_NoData(ix, iy) && (d = pSurface->asDouble(ix, iy)) < z )
					    {
						    if( bLinearFlow && linearCtrl > thresLinear )	// D8
						    {
							    dzSum = (z - d) / Get_Length(i);
							    if( maxGrad < dzSum )
							    {
								    maxGrad = dzSum;
								    steepestN = i;
							    }
						    }
						    else											// MFD Freeman 1991
						    {
							    gradient = (z - d) / Get_Length(i);
							    dzSum	+= (dz[i]	= pow(gradient, Convergence));
						    }
					    }
					    else
					    {
						    dz[i]	= 0.0;
					    }
				    }
				    else
					    bBorder = true;
			    }

			
			    // routing
			    //-----------------------------------------------------
			    if( dzSum > 0.0 && !bBorder )
			    {
				    if( bLinearFlow && linearCtrl > thresLinear )
				    {
					    ix	= Get_xTo(steepestN, x);
					    iy	= Get_yTo(steepestN, y);
	
					    pFlux->Add_Value(ix, iy, flux);	
				    }
				    else
				    {
					    d_flux	=  flux / dzSum;

					    for(int i=0; i<8; i++)
					    {
						    if( dz[i] > 0.0 )
						    {
							    ix	= Get_xTo(i, x);
							    iy	= Get_yTo(i, y);

							    pFlux->Add_Value(ix, iy, d_flux * dz[i]);
						    }
					    }
				    }
			    }

			    pFlux->Set_Value(x, y, flux);

			    if( pStateOut != NULL )
                {
				    pStateOut->Set_Value(x, y, state);
                }
		    }
        }
	}// for

	DataObject_Set_Colors(pFlux, 11, SG_COLORS_WHITE_BLUE);

	if( pStateOut != NULL )
		DataObject_Set_Colors(pStateOut, 11, SG_COLORS_WHITE_RED);

	//-----------------------------------------------------
	return( true );
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
