
///////////////////////////////////////////////////////////
//                                                       //
//                         SAGA                          //
//                                                       //
//      System for Automated Geoscientific Analyses      //
//                                                       //
//                     Tool Library                      //
//                       3d_viewer                       //
//                                                       //
//-------------------------------------------------------//
//                                                       //
//                   3d_viewer_tin.cpp                   //
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
#include <wx/menu.h>

#include "3d_viewer_tin.h"


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
class C3D_Viewer_TIN_Panel : public CSG_3DView_Panel
{
public:
	C3D_Viewer_TIN_Panel(wxWindow *pParent, CSG_TIN *pTIN, int Field_Z, int Field_Color, CSG_Grid *pDrape);


protected:

	virtual int					On_Parameters_Enable	(CSG_Parameters *pParameters, CSG_Parameter *pParameter);

	virtual void				Update_Statistics		(void);
	virtual void				Update_Parent			(void);

	virtual void				On_Key_Down				(wxKeyEvent   &event);

	virtual bool				On_Before_Draw			(void);
	virtual bool				On_Draw					(void);

	virtual int					Get_Color				(double Value);


private:

	bool						m_Color_bGrad;

	double						m_Color_Min, m_Color_Scale;

	CSG_Colors					m_Colors;

	CSG_TIN						*m_pTIN;


	//-----------------------------------------------------
	DECLARE_EVENT_TABLE()

};


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
BEGIN_EVENT_TABLE(C3D_Viewer_TIN_Panel, CSG_3DView_Panel)
	EVT_KEY_DOWN	(C3D_Viewer_TIN_Panel::On_Key_Down)
END_EVENT_TABLE()


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
C3D_Viewer_TIN_Panel::C3D_Viewer_TIN_Panel(wxWindow *pParent, CSG_TIN *pTIN, int zField, int cField, CSG_Grid *pDrape)
	: CSG_3DView_Panel(pParent, pDrape)
{
	m_pTIN	= pTIN;

	//-----------------------------------------------------
	CSG_String	Attributes;

	for(int i=0; i<pTIN->Get_Field_Count(); i++)
	{
		Attributes	+= pTIN->Get_Field_Name(i);
		Attributes	+= "|";
	}

	//-----------------------------------------------------
	m_Parameters.Add_Choice("NODE_GENERAL",
		"Z_ATTR"		, _TL("Z Attribute"),
		_TL(""),
		Attributes, zField
	);

	m_Parameters.Add_Double("Z_ATTR",
		"Z_SCALE"		, _TL("Exaggeration"),
		_TL(""),
		1.
	);

	//-----------------------------------------------------
	m_Parameters.Add_Node("",
		"NODE_VIEW"		, _TL("TIN View Settings"),
		_TL("")
	);

	m_Parameters.Add_Bool("NODE_VIEW",
		"DRAW_FACES"	, _TL("Draw Faces"),
		_TL(""),
		true
	);

	m_Parameters.Add_Choice("DRAW_FACES",
		"COLORS_ATTR"	, _TL("Colour Attribute"),
		_TL(""),
		Attributes, cField
	);

	m_Parameters.Add_Colors("COLORS_ATTR",
		"COLORS"		, _TL("Colours"),
		_TL("")
	);

	m_Parameters.Add_Bool("COLORS_ATTR",
		"COLORS_GRAD"	, _TL("Graduated"),
		_TL(""),
		true
	);

	m_Parameters.Add_Range("COLORS_ATTR",
		"COLORS_RANGE"	, _TL("Value Range"),
		_TL("")
	);

	m_Parameters.Add_Choice("DRAW_FACES",
		"SHADING"		, _TL("Shading"),
		_TL(""),
		CSG_String::Format("%s|%s",
			_TL("none"),
			_TL("shading")
		), 1
	);

	m_Parameters.Add_Double("SHADING",
		"SHADE_DEC"		, _TL("Light Source Height"),
		_TL(""),
		45., -90., true, 90., true
	);

	m_Parameters.Add_Double("SHADING",
		"SHADE_AZI"		, _TL("Light Source Direction"),
		_TL(""),
		90., 0., true, 360., true
	);

	//-----------------------------------------------------
	m_Parameters.Add_Bool("NODE_VIEW",
		"DRAW_EDGES"	, _TL("Draw Wire"),
		_TL(""),
		false
	);

	m_Parameters.Add_Bool("DRAW_EDGES",
		"EDGE_COLOR_UNI", _TL("Single Colour"),
		_TL(""),
		false
	);

	m_Parameters.Add_Color("EDGE_COLOR_UNI",
		"EDGE_COLOR"	, _TL("Colour"),
		_TL(""),
		SG_GET_RGB(150, 150, 150)
	);

	//-----------------------------------------------------
	m_Parameters.Add_Bool("",
		"DRAW_NODES"	, _TL("Draw Nodes"),
		_TL(""),
		false
	);

	m_Parameters.Add_Color("DRAW_NODES",
		"NODE_COLOR"	, _TL("Colour"),
		_TL(""),
		SG_COLOR_BLACK
	);

	m_Parameters.Add_Int("DRAW_NODES",
		"NODE_SIZE"		, _TL("Size"),
		_TL(""),
		2, 1, true
	);

	m_Parameters.Add_Double("DRAW_NODES",
		"NODE_SCALE"	, _TL("Size Scaling"),
		_TL(""),
		250., 1., true
	);

	//-----------------------------------------------------
	Update_Statistics();
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
int C3D_Viewer_TIN_Panel::On_Parameters_Enable(CSG_Parameters *pParameters, CSG_Parameter *pParameter)
{
	if( pParameter->Cmp_Identifier("DRAW_FACES") )
	{
		CSG_Parameter *pDrape = pParameters->Get_Parameter("DO_DRAPE");

		pParameters->Set_Enabled("COLORS_ATTR"   , pParameter->asBool() && (!pDrape || !pDrape->asBool()));
		pParameters->Set_Enabled("SHADING"       , pParameter->asBool());
	}

	if( pParameter->Cmp_Identifier("DO_DRAPE") )
	{
		CSG_Parameter *pFaces = pParameters->Get_Parameter("DRAW_FACES");

		pParameters->Set_Enabled("COLORS_ATTR"   , pParameter->asBool() == false && pFaces->asBool() == true);
	}

	if( pParameter->Cmp_Identifier("SHADING") )
	{
		pParameters->Set_Enabled("SHADE_DEC"     , pParameter->asBool());
		pParameters->Set_Enabled("SHADE_AZI"     , pParameter->asBool());
	}

	if( pParameter->Cmp_Identifier("DRAW_EDGES") )
	{
		pParameters->Set_Enabled("EDGE_COLOR_UNI", pParameter->asBool());
	}

	if( pParameter->Cmp_Identifier("EDGE_COLOR_UNI") )
	{
		pParameters->Set_Enabled("EDGE_COLOR"   , pParameter->asBool());
	}

	if( pParameter->Cmp_Identifier("DRAW_NODES") )
	{
		pParameters->Set_Enabled("NODE_COLOR"   , pParameter->asBool());
		pParameters->Set_Enabled("NODE_SIZE"    , pParameter->asBool());
		pParameters->Set_Enabled("NODE_SCALE"   , pParameter->asBool());
	}

	return( CSG_3DView_Panel::On_Parameters_Enable(pParameters, pParameter) );
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
void C3D_Viewer_TIN_Panel::Update_Statistics(void)
{
	int	Field	= m_Parameters("COLORS_ATTR")->asInt();

	m_Parameters("COLORS_RANGE")->asRange()->Set_Range(
		m_pTIN->Get_Mean(Field) - 1.5 * m_pTIN->Get_StdDev(Field),
		m_pTIN->Get_Mean(Field) + 1.5 * m_pTIN->Get_StdDev(Field)
	);

	m_Data_Min.x	= m_pTIN->Get_Extent().Get_XMin();
	m_Data_Max.x	= m_pTIN->Get_Extent().Get_XMax();

	m_Data_Min.y	= m_pTIN->Get_Extent().Get_YMin();
	m_Data_Max.y	= m_pTIN->Get_Extent().Get_YMax();

	m_Data_Min.z	= m_pTIN->Get_Minimum(m_Parameters("Z_ATTR")->asInt());
	m_Data_Max.z	= m_pTIN->Get_Maximum(m_Parameters("Z_ATTR")->asInt());

	//-----------------------------------------------------
	Update_View();
}

//---------------------------------------------------------
void C3D_Viewer_TIN_Panel::Update_Parent(void)
{
	((CSG_3DView_Dialog *)GetParent())->Update_Controls();
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
void C3D_Viewer_TIN_Panel::On_Key_Down(wxKeyEvent &event)
{
	switch( event.GetKeyCode() )
	{
	default:
		CSG_3DView_Panel::On_Key_Down(event);
		return;

	case WXK_F1: m_Parameters("Z_SCALE"   )->Set_Value(m_Parameters("Z_SCALE"   )->asDouble() -  0.5); break;
	case WXK_F2: m_Parameters("Z_SCALE"   )->Set_Value(m_Parameters("Z_SCALE"   )->asDouble() +  0.5); break;

	case WXK_F5: m_Parameters("NODE_SIZE" )->Set_Value(m_Parameters("NODE_SIZE" )->asDouble() -  1.0); break;
	case WXK_F6: m_Parameters("NODE_SIZE" )->Set_Value(m_Parameters("NODE_SIZE" )->asDouble() +  1.0); break;

	case WXK_F7: m_Parameters("NODE_SCALE")->Set_Value(m_Parameters("NODE_SCALE")->asDouble() - 10.0); break;
	case WXK_F8: m_Parameters("NODE_SCALE")->Set_Value(m_Parameters("NODE_SCALE")->asDouble() + 10.0); break;
	}

	//-----------------------------------------------------
	Update_View();
	Update_Parent();
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool C3D_Viewer_TIN_Panel::On_Before_Draw(void)
{
	if( m_Play_State == SG_3DVIEW_PLAY_STOP )
	{
		m_Projector.Set_zScaling(m_Projector.Get_xScaling() * m_Parameters("Z_SCALE")->asDouble());
	}

	return( true );
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
int C3D_Viewer_TIN_Panel::Get_Color(double Value)
{
	if( m_Color_Scale <= 0. )
	{
		return( (int)Value );
	}

	double	c	= m_Color_Scale * (Value - m_Color_Min);

	return( m_Color_bGrad ? m_Colors.Get_Interpolated(c) : m_Colors[(int)c] );
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool C3D_Viewer_TIN_Panel::On_Draw(void)
{
	int zField = m_Parameters("Z_ATTR"     )->asInt();
	int cField = m_Parameters("COLORS_ATTR")->asInt();

	if( m_Parameters("COLORS_RANGE")->asRange()->Get_Min()
	>=  m_Parameters("COLORS_RANGE")->asRange()->Get_Max() )
	{
		m_Parameters("COLORS_RANGE")->asRange()->Set_Range(
			m_pTIN->Get_Mean(cField) - 1.5 * m_pTIN->Get_StdDev(cField),
			m_pTIN->Get_Mean(cField) + 1.5 * m_pTIN->Get_StdDev(cField)
		);
	}

	m_Colors      = *m_Parameters("COLORS")->asColors();
	m_Color_bGrad = m_Parameters("COLORS_GRAD")->asBool();
	m_Color_Min   = m_Parameters("COLORS_RANGE")->asRange()->Get_Min();
	m_Color_Scale = m_Colors.Get_Count() / (m_Parameters("COLORS_RANGE")->asRange()->Get_Max() - m_Color_Min);

	//-----------------------------------------------------
	if( m_Parameters("DRAW_FACES")->asBool() )	// Face
	{
		bool bDrape = m_Parameters("DO_DRAPE") && m_Parameters("DO_DRAPE")->asBool();

		CSG_Vector LightSource;

		if( m_Parameters("SHADING")->asInt() && LightSource.Create(3) )
		{
			double decline = m_Parameters("SHADE_DEC")->asDouble() * -M_DEG_TO_RAD;
			double azimuth = m_Parameters("SHADE_AZI")->asDouble() *  M_DEG_TO_RAD;

			LightSource[0] = sin(decline) * cos(azimuth);
			LightSource[1] = sin(decline) * sin(azimuth);
			LightSource[2] = cos(decline);
		}

		#pragma omp parallel for
		for(int iTriangle=0; iTriangle<m_pTIN->Get_Triangle_Count(); iTriangle++)
		{
			CSG_TIN_Triangle *pTriangle = m_pTIN->Get_Triangle(iTriangle); TSG_Triangle_Node p[3];

			for(int i=0; i<3; i++)
			{
				CSG_TIN_Node *pNode = pTriangle->Get_Node(i);

				p[i].x = pNode->Get_Point().x;
				p[i].y = pNode->Get_Point().y;
				p[i].z = pNode->asDouble(zField);

				if( bDrape )
				{
					p[i].c = pNode->Get_Point().x;
					p[i].d = pNode->Get_Point().y;
				}
				else
				{
					p[i].c = pNode->asDouble(cField);
				}

				m_Projector.Get_Projection(p[i].x, p[i].y, p[i].z);
			}

			//---------------------------------------------
			if( LightSource.Get_Size() )
				Draw_Triangle(p, false, LightSource);
			else
				Draw_Triangle(p, false);
		}
	}

	//-----------------------------------------------------
	if( m_Parameters("DRAW_EDGES")->asBool() )	// Edges
	{
		bool bColor = m_Parameters("EDGE_COLOR_UNI")->asBool();
		int  Color  = m_Parameters("EDGE_COLOR"    )->asColor();

		#pragma omp parallel for
		for(int iEdge=0; iEdge<m_pTIN->Get_Edge_Count(); iEdge++)
		{
			CSG_TIN_Edge *pEdge = m_pTIN->Get_Edge(iEdge); TSG_Triangle_Node p[2];

			for(int i=0; i<2; i++)
			{
				CSG_TIN_Node *pNode = pEdge->Get_Node(i);

				p[i].x = pNode->Get_Point().x;
				p[i].y = pNode->Get_Point().y;
				p[i].z = pNode->asDouble(zField);
				p[i].c = Get_Color(pNode->asDouble(cField));

				m_Projector.Get_Projection(p[i].x, p[i].y, p[i].z);
			}

			//---------------------------------------------
			if( bColor )
			{
				Draw_Line(p[0].x, p[0].y, p[0].z, p[1].x, p[1].y, p[1].z, Color);
			}
			else
			{
				Draw_Line(p[0].x, p[0].y, p[0].z, p[1].x, p[1].y, p[1].z, p[0].c, p[1].c);
			}
		}
	}

	//-------------------------------------------------
	if( m_Parameters("DRAW_NODES")->asBool() )	// Nodes
	{
		int Color = m_Parameters("NODE_COLOR")->asColor ();
		int Size  = m_Parameters("NODE_SIZE" )->asInt   ();

		#pragma omp parallel for
		for(int iNode=0; iNode<m_pTIN->Get_Node_Count(); iNode++)
		{
			CSG_TIN_Node *pNode = m_pTIN->Get_Node(iNode); TSG_Point_Z p;

			p.x = pNode->Get_Point().x;
			p.y = pNode->Get_Point().y;
			p.z = pNode->asDouble(zField);

			m_Projector.Get_Projection(p.x, p.y, p.z);

			Draw_Point(p.x, p.y, p.z, Color, Size);
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
class C3D_Viewer_TIN_Dialog : public CSG_3DView_Dialog
{
public:
	C3D_Viewer_TIN_Dialog(CSG_TIN *pTIN, int Field_Z, int Field_Color, CSG_Grid *pDrape);

	virtual void				Update_Controls			(void);


protected:

	wxChoice					*m_pField_Z, *m_pField_C;

//	wxCheckBox					*m_pFaces, *m_pEdges, *m_pNodes;


	virtual void				On_Update_Choices		(wxCommandEvent &event);
//	virtual void				On_Update_Control		(wxCommandEvent &event);

	virtual void				Set_Menu				(wxMenu &Menu);
	virtual void				On_Menu					(wxCommandEvent &event);
	virtual void				On_Menu_UI				(wxUpdateUIEvent &event);


private:

	DECLARE_EVENT_TABLE()

};

//---------------------------------------------------------
BEGIN_EVENT_TABLE(C3D_Viewer_TIN_Dialog, CSG_3DView_Dialog)
	EVT_CHOICE		(wxID_ANY, C3D_Viewer_TIN_Dialog::On_Update_Choices)
//	EVT_CHECKBOX	(wxID_ANY, C3D_Viewer_TIN_Dialog::On_Update_Control)
	EVT_MENU_RANGE		(MENU_USER_FIRST, MENU_USER_LAST, C3D_Viewer_TIN_Dialog::On_Menu)
	EVT_UPDATE_UI_RANGE	(MENU_USER_FIRST, MENU_USER_LAST, C3D_Viewer_TIN_Dialog::On_Menu_UI)
END_EVENT_TABLE()

//---------------------------------------------------------
C3D_Viewer_TIN_Dialog::C3D_Viewer_TIN_Dialog(CSG_TIN *pTIN, int Field_Z, int Field_Color, CSG_Grid *pDrape)
	: CSG_3DView_Dialog(_TL("TIN Viewer"))
{
	Create(new C3D_Viewer_TIN_Panel(this, pTIN, Field_Z, Field_Color, pDrape));

	wxArrayString	Attributes;

	for(int i=0; i<pTIN->Get_Field_Count(); i++)
	{
		Attributes.Add(pTIN->Get_Field_Name(i));
	}

	Add_Spacer();
	m_pField_Z	= Add_Choice  (_TL("Elevation"), Attributes, Field_Z);
	m_pField_C	= Add_Choice  (_TL("Colour"   ), Attributes, Field_Color);

//	Add_Spacer();
//	m_pFaces	= Add_CheckBox(_TL("Faces"), m_pPanel->m_Parameters("DRAW_FACES")->asBool());
//	m_pEdges	= Add_CheckBox(_TL("Edges"), m_pPanel->m_Parameters("DRAW_EDGES")->asBool());
//	m_pNodes	= Add_CheckBox(_TL("Nodes"), m_pPanel->m_Parameters("DRAW_NODES")->asBool());
}

//---------------------------------------------------------
void C3D_Viewer_TIN_Dialog::On_Update_Choices(wxCommandEvent &event)
{
	if( event.GetEventObject() == m_pField_Z )
	{
		m_pPanel->m_Parameters("Z_ATTR")->Set_Value(m_pField_Z->GetSelection());
		m_pPanel->Update_View(true);
		return;
	}

	if( event.GetEventObject() == m_pField_C )
	{
		m_pPanel->m_Parameters("COLORS_ATTR")->Set_Value(m_pField_C->GetSelection());
		m_pPanel->Update_View(true);
		return;
	}

	CSG_3DView_Dialog::On_Update_Choices(event);
}

//---------------------------------------------------------
//void C3D_Viewer_TIN_Dialog::On_Update_Control(wxCommandEvent &event)
//{
//	CHECKBOX_UPDATE(m_pFaces, "DRAW_FACES");
//	CHECKBOX_UPDATE(m_pEdges, "DRAW_EDGES");
//	CHECKBOX_UPDATE(m_pNodes, "DRAW_NODES");

//	CSG_3DView_Dialog::On_Update_Control(event);
//}

//---------------------------------------------------------
void C3D_Viewer_TIN_Dialog::Update_Controls(void)
{
	m_pField_Z->SetSelection(m_pPanel->m_Parameters("Z_ATTR"     )->asInt());
	m_pField_C->SetSelection(m_pPanel->m_Parameters("COLORS_ATTR")->asInt());

//	m_pFaces->SetValue(m_pPanel->m_Parameters("DRAW_FACES")->asBool());
//	m_pEdges->SetValue(m_pPanel->m_Parameters("DRAW_EDGES")->asBool());
//	m_pNodes->SetValue(m_pPanel->m_Parameters("DRAW_NODES")->asBool());

	CSG_3DView_Dialog::Update_Controls();
}

//---------------------------------------------------------
enum
{
	MENU_SCALE_Z_DEC = MENU_USER_FIRST,
	MENU_SCALE_Z_INC,
	MENU_COLORS_GRAD,
	MENU_SHADING,
	MENU_FACES,
	MENU_EDGES,
	MENU_NODES
};

//---------------------------------------------------------
void C3D_Viewer_TIN_Dialog::Set_Menu(wxMenu &Menu)
{
	wxMenu	*pMenu	= Menu.FindChildItem(Menu.FindItem(_TL("Display")))->GetSubMenu();

	pMenu->AppendSeparator();
	pMenu->Append(MENU_SCALE_Z_DEC, _TL("Decrease Exaggeration [F1]"));
	pMenu->Append(MENU_SCALE_Z_INC, _TL("Increase Exaggeration [F2]"));

	pMenu->AppendSeparator();
	pMenu->AppendCheckItem(MENU_COLORS_GRAD, _TL("Graduated Colours"));

	Menu.AppendSeparator();
	Menu.AppendCheckItem(MENU_SHADING, _TL("Shading"));

	Menu.AppendSeparator();
	Menu.AppendCheckItem(MENU_FACES, _TL("Faces"));
	Menu.AppendCheckItem(MENU_EDGES, _TL("Edges"));
	Menu.AppendCheckItem(MENU_NODES, _TL("Nodes"));
}

//---------------------------------------------------------
void C3D_Viewer_TIN_Dialog::On_Menu(wxCommandEvent &event)
{
	switch( event.GetId() )
	{
	case MENU_SCALE_Z_DEC: m_pPanel->m_Parameters("Z_SCALE")->Set_Value(m_pPanel->m_Parameters("Z_SCALE")->asDouble() - 0.5); m_pPanel->Update_View();	return;
	case MENU_SCALE_Z_INC: m_pPanel->m_Parameters("Z_SCALE")->Set_Value(m_pPanel->m_Parameters("Z_SCALE")->asDouble() + 0.5); m_pPanel->Update_View();	return;

	case MENU_COLORS_GRAD: MENU_TOGGLE("COLORS_GRAD");	return;

	case MENU_SHADING    : MENU_TOGGLE("SHADING"    );	return;

	case MENU_FACES      : MENU_TOGGLE("DRAW_FACES" );	return;
	case MENU_EDGES      : MENU_TOGGLE("DRAW_EDGES" );	return;
	case MENU_NODES      : MENU_TOGGLE("DRAW_NODES" );	return;
	}

	CSG_3DView_Dialog::On_Menu(event);
}

//---------------------------------------------------------
void C3D_Viewer_TIN_Dialog::On_Menu_UI(wxUpdateUIEvent &event)
{
	switch( event.GetId() )
	{
	case MENU_COLORS_GRAD: event.Check(m_pPanel->m_Parameters("COLORS_GRAD")->asBool());	return;

	case MENU_SHADING    : event.Check(m_pPanel->m_Parameters("SHADING"    )->asBool());	return;

	case MENU_FACES      : event.Check(m_pPanel->m_Parameters("DRAW_FACES" )->asBool());	return;
	case MENU_EDGES      : event.Check(m_pPanel->m_Parameters("DRAW_EDGES" )->asBool());	return;
	case MENU_NODES      : event.Check(m_pPanel->m_Parameters("DRAW_NODES" )->asBool());	return;
	}

	CSG_3DView_Dialog::On_Menu_UI(event);
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
C3D_Viewer_TIN::C3D_Viewer_TIN(void)
{
	Set_Name		(_TL("TIN Viewer"));

	Set_Author		("O.Conrad (c) 2014");

	Set_Description	(_TW(
		"3D viewer for TIN."
	));

	//-----------------------------------------------------
	Parameters.Add_TIN("",
		"TIN"		, _TL("TIN"),
		_TL(""),
		PARAMETER_INPUT
	);

	Parameters.Add_Table_Field("TIN",
		"HEIGHT"	, _TL("Elevation"),
		_TL("")
	);

	Parameters.Add_Table_Field("TIN",
		"COLOR"		, _TL("Color"),
		_TL("")
	);

	Parameters.Add_Grid("",
		"DRAPE"		, _TL("Map"),
		_TL("rgb coded raster map to be draped"),
		PARAMETER_INPUT_OPTIONAL
	);
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool C3D_Viewer_TIN::On_Execute(void)
{
	CSG_TIN *pTIN = Parameters("TIN")->asTIN();

	if( !pTIN->is_Valid() )
	{
		Error_Set(_TL("invalid input data"));

		return( false );
	}

	C3D_Viewer_TIN_Dialog dlg(pTIN,
		Parameters("HEIGHT")->asInt(),
		Parameters("COLOR" )->asInt(),
		Parameters("DRAPE" )->asGrid()
	);

	dlg.ShowModal();

	return( true );
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
