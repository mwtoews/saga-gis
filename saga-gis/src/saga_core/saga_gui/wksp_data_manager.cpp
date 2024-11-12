
///////////////////////////////////////////////////////////
//                                                       //
//                         SAGA                          //
//                                                       //
//      System for Automated Geoscientific Analyses      //
//                                                       //
//                    User Interface                     //
//                                                       //
//                    Program: SAGA                      //
//                                                       //
//-------------------------------------------------------//
//                                                       //
//                 WKSP_Data_Manager.cpp                 //
//                                                       //
//          Copyright (C) 2005 by Olaf Conrad            //
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
//    contact:    Olaf Conrad                            //
//                Institute of Geography                 //
//                University of Goettingen               //
//                Goldschmidtstr. 5                      //
//                37077 Goettingen                       //
//                Germany                                //
//                                                       //
//    e-mail:     oconrad@saga-gis.org                   //
//                                                       //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#ifdef _SAGA_LINUX
#include <stdlib.h>
#endif

#include <wx/filename.h>

#include <saga_api/saga_api.h>

#include "saga.h"
#include "saga_frame.h"

#include "res_commands.h"
#include "res_dialogs.h"

#include "helper.h"
#include "project.h"

#include "wksp_data_control.h"
#include "wksp_data_manager.h"
#include "wksp_data_menu_files.h"
#include "wksp_data_layers.h"

#include "wksp_layer.h"

#include "wksp_table_manager.h"
#include "wksp_table.h"
#include "wksp_shapes_manager.h"
#include "wksp_shapes.h"
#include "wksp_tin_manager.h"
#include "wksp_pointcloud_manager.h"
#include "wksp_grid_manager.h"

#include "wksp_map_manager.h"

#include "wksp_tool_manager.h"
#include "wksp_tool.h"

#include "active.h"
#include "active_parameters.h"


///////////////////////////////////////////////////////////
//                                                       //
//                                                       //
//                                                       //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
CWKSP_Data_Manager	*g_pData	= NULL;


///////////////////////////////////////////////////////////
//                                                       //
//                                                       //
//                                                       //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
CWKSP_Data_Manager::CWKSP_Data_Manager(void)
{
	g_pData			= this;

	m_pTables		= NULL;
	m_pShapes		= NULL;
	m_pTINs			= NULL;
	m_pPointClouds	= NULL;
	m_pGrids		= NULL;

	m_pProject		= new CWKSP_Project;
	m_pMenu_Files	= new CWKSP_Data_Menu_Files;

	//-----------------------------------------------------
	m_Parameters.Add_Bool (""          , "THUMBNAILS"        , _TL("Thumbnails"     ), _TL(""), true);
	m_Parameters.Add_Int  ("THUMBNAILS", "THUMBNAIL_SIZE"    , _TL("Size"           ), _TL(""), 50, 10, true);
	m_Parameters.Add_Color("THUMBNAILS", "THUMBNAIL_SELCOLOR", _TL("Selection Color"), _TL(""), Get_Color_asInt(SYS_Get_Color(wxSYS_COLOUR_BTNSHADOW)));
	m_Parameters.Add_Bool ("THUMBNAILS", "THUMBNAIL_CATEGORY", _TL("Show Categories"), _TL(""), true);

	m_Parameters.Add_Bool("",
		"SHOW_FILE_SOURCES"		, _TL("Show Data File Source Browser"),
		_TL("Show data sources tab for file system. Disabling might speed up start-up. Changes take effect after restart."),
		true
	);

	//-----------------------------------------------------
	m_Parameters.Add_Node("", "NODE_PROJECT", _TL("Projects"), _TL(""));

	m_Parameters.Add_Choice("NODE_PROJECT",
		"PROJECT_START"			, _TL("Startup Project"),
		_TL(""),
		CSG_String::Format("%s|%s|%s",
			_TL("empty"),
			_TL("last state"),
			_TL("ask what to do")
		), 2
	);

	m_Parameters.Add_Choice("NODE_PROJECT",
		"PROJECT_MAP_ARRANGE", _TL("Map Window Arrangement"),
		_TL("Initial map window arrangement after a project has been loaded."),
		#ifndef MDI_TABBED
		CSG_String::Format("%s|%s|%s|%s|%s",
			_TL("Cascade"),
			_TL("Tile Horizontally"),
			_TL("Tile Vertically"),
			_TL("Maximized"),
			_TL("Don't Restore")
		), 2
		#else
		CSG_String::Format("%s|%s",
			_TL("Open Map Windows"),
			_TL("Don't Open Map Windows")
		), 0
		#endif
	);

	//-----------------------------------------------------
	m_Parameters.Add_Node("", "NODE_DB", _TL("Databases"), _TL(""));

	m_Parameters.Add_Choice("NODE_DB",
		"PROJECT_DB_REOPEN"		, _TL("Reopen Database Connections"),
		_TL("Reopen PostgreSQL database connections. Warning: if set to true account information including unencrypted passwords for automatic connection will be stored."),
		CSG_String::Format("%s|%s",
			_TL("no"),
			_TL("yes")
		), 0
	);

	m_Parameters.Add_Int("PROJECT_DB_REOPEN",
		"PROJECT_DB_WAIT"		, _TL("Response Time"),
		_TL("Maximum time (seconds) to wait for server response. If zero it waits until the PostgreSQL connection is established or refused."),
		2, 0, true
	);

	//-----------------------------------------------------
	m_Parameters.Add_Node("", "NODE_DATA", _TL("Data"), _TL(""));

	m_Parameters.Add_Int("NODE_DATA",
		"NUMBERING"				, _TL("Data Set Numeration"),
		_TL("Minimum width of data set numbering. If set to 0 no numbering is applied at all. Negative values will prepend zeros. Numbering helps in data set sorting and selection."),
		3
	);

	m_Parameters.Add_Bool("NODE_DATA",
		"NAME_BY_FILE"			, _TL("Use File Name for Naming"),
		_TL("Use file name to name a data set, when it has been loaded from file."),
		false
	);

	m_Parameters.Add_Bool("NODE_DATA",
		"COLORS_FROM_TOOL"		, _TL("Tool Set Colors"),
		_TL("Allow tools to change data set colors programmatically."),
		true
	);

	m_Parameters.Add_Colors("NODE_DATA",
		"COLORS_DEFAULT"		, _TL("Default Colors"),
		_TL("")
	);

	m_Parameters.Add_Choice("NODE_DATA"      , "STRETCH_DEFAULT", _TL("Histogram Stretch Defaults"), _TL("Histogram stretch applied by default to new data."), CSG_String::Format("%s|%s|%s",
		_TL("Linear"), _TL("Standard Deviation"), _TL("Percent Clip")), 1
	);

	m_Parameters.Add_Range ("STRETCH_DEFAULT", "STRETCH_LINEAR" , _TL("Linear"                    ), _TL("Linear percent stretch allows you to trim extreme values from both ends of the histogram using the percentage specified here."), 5., 95., 0., true, 100., true);
	m_Parameters.Add_Double("STRETCH_DEFAULT", "STRETCH_STDDEV" , _TL("Standard Deviation"        ), _TL(""), 2., 0., true);
	m_Parameters.Add_Range ("STRETCH_DEFAULT", "STRETCH_PCTL"   , _TL("Percent Clip"              ), _TL(""), 2., 98., 0., true, 100., true);

	SG_DataObject_Set_Max_Samples(1000000);	// default to one million

	m_Parameters.Add_Int("NODE_DATA",
		"DATA_SAMPLE_MAX"		, _TL("Maximum Samples"),
		_TL("Default maximum number of samples used to build statistics and histograms. Set to zero to use all data records."),
		(int)SG_DataObject_Get_Max_Samples(), 0, true
	);

	//-----------------------------------------------------
	m_Parameters.Add_Int("NODE_DATA",
		"HISTORY_DEPTH"			, _TL("History Depth"),
		_TL("Depth to which data history is stored. Set -1 keeps all history entries (default), 0 switches history option off."),
		SG_Get_History_Depth(), -1, true
	);

	m_Parameters.Add_Bool("HISTORY_DEPTH",
		"HISTORY_LISTS"			, _TL("Ignore Input Lists"),
		_TL(""),
		SG_Get_History_Ignore_Lists() != 0
	);

	//-----------------------------------------------------
	m_Parameters.Add_Node("", "NODE_GRID", _TL("Grids"), _TL(""));

	m_Parameters.Add_Choice("NODE_GRID",
		"GRID_FMT_DEFAULT"		, _TL("Default Output Format"),
		_TL(""),
		CSG_String::Format("%s (*.sg-grd-z)|%s (*.sg-grd)|%s (*.tif)",
			_TL("SAGA Compressed Grid Files"),
			_TL("SAGA Grid Files"),
			_TL("GeoTIFF")
		), 0
	);

	m_Parameters.Add_Int("NODE_GRID",
		"GRID_COORD_PRECISION"	, _TL("Coordinate Precision"),
		_TL("Precision used to store coordinates and cell sizes (i.e. number of decimals). Ignored if negative."),
		10, -1, true
	);

	m_Parameters.Add_Int("NODE_GRID",
		"GRID_SELECT_MAX"		, _TL("Maximum Selection"),
		_TL("Maximum number of rows/columns in selection of grid cells."),
		100, 1, true
	);

	m_Parameters.Add_Choice("NODE_GRID",
		"GRID_CACHE_MODE"		, _TL("File Cache"),
		_TL("Activate file caching automatically, if memory size exceeds the threshold value."),
		CSG_String::Format("%s|%s|%s",
			_TL("no"),
			_TL("yes"),
			_TL("after confirmation")
		), SG_Grid_Cache_Get_Mode()
	);

	m_Parameters.Add_Double("GRID_CACHE_MODE",
		"GRID_CACHE_THRSHLD"	, _TL("Threshold for automatic mode [MB]"),
		_TL(""),
		SG_Grid_Cache_Get_Threshold_MB(), 0., true
	);

	m_Parameters.Add_FilePath("GRID_CACHE_MODE",
		"GRID_CACHE_TMPDIR"		, _TL("Temporary files"),
		_TL("Directory, where temporary cache files shall be saved."),
		NULL, SG_Grid_Cache_Get_Directory(), true, true
	);

	//-----------------------------------------------------
	m_Parameters.Add_Node("", "NODE_TABLE", _TL("Tables"), _TL(""));

	m_Parameters.Add_Choice("NODE_TABLE",
		"TABLE_FLT_STYLE"		, _TL("Floating Point Numbers"),
		_TL(""),
		CSG_String::Format("%s|%s|%s",
			_TL("system default"),
			_TL("compact"),
			_TL("fix number of decimals")
		), 1
	);

	m_Parameters.Add_Int("NODE_TABLE",
		"TABLE_FLT_DECIMALS"	, _TL("Decimals"),
		_TL(""),
		6, 0, true
	);

	//-----------------------------------------------------
	m_Parameters.Add_Node("", "NODE_SHAPES", _TL("Shapes"), _TL(""));

	m_Parameters.Add_Choice("NODE_SHAPES",
		"SHAPES_FMT_DEFAULT"	, _TL("Default Output Format"),
		_TL(""),
		CSG_String::Format("%s|%s|%s",
			_TL("ESRI Shape File (*.shp)"),
			_TL("GeoPackage (*.gpkg)"),
			_TL("GeoJSON (*.geojson)")
		), 0
	);

	//-----------------------------------------------------
	CONFIG_Read("/DATA", &m_Parameters);

	SG_DataObject_Set_Max_Samples    (m_Parameters("DATA_SAMPLE_MAX"     )->asInt   ());

	SG_Grid_Cache_Set_Mode           (m_Parameters("GRID_CACHE_MODE"     )->asInt   ());
	SG_Grid_Cache_Set_Threshold_MB   (m_Parameters("GRID_CACHE_THRSHLD"  )->asDouble());
	SG_Grid_Cache_Set_Directory      (m_Parameters("GRID_CACHE_TMPDIR"   )->asString());

	CSG_Grid_System::Set_Precision   (m_Parameters("GRID_COORD_PRECISION")->asInt   ());

	SG_Set_History_Depth             (m_Parameters("HISTORY_DEPTH"       )->asInt   ());
	SG_Set_History_Ignore_Lists      (m_Parameters("HISTORY_LISTS"       )->asInt   ());

	switch( m_Parameters("GRID_FMT_DEFAULT")->asInt() )
	{
	default: SG_Grid_Set_File_Format_Default(GRID_FILE_FORMAT_Compressed); break;
	case  1: SG_Grid_Set_File_Format_Default(GRID_FILE_FORMAT_Binary    ); break;
	case  2: SG_Grid_Set_File_Format_Default(GRID_FILE_FORMAT_GeoTIFF   ); break;
	}

	switch( m_Parameters("SHAPES_FMT_DEFAULT")->asInt() )
	{
	default: SG_Shapes_Set_File_Format_Default(SHAPE_FILE_FORMAT_ESRI      ); break;
	case  1: SG_Shapes_Set_File_Format_Default(SHAPE_FILE_FORMAT_GeoPackage); break;
	case  2: SG_Shapes_Set_File_Format_Default(SHAPE_FILE_FORMAT_GeoJSON   ); break;
	}
}

//---------------------------------------------------------
CWKSP_Data_Manager::~CWKSP_Data_Manager(void)
{
	CONFIG_Write("/DATA", &m_Parameters);

	delete(m_pProject);
	delete(m_pMenu_Files);

	g_pData	= NULL;
}


///////////////////////////////////////////////////////////
//                                                       //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
wxFileName Get_SAGA_GUI_CFG(void)
{
#ifdef _SAGA_LINUX
//	wxFileName	fProject(wxString(getenv( "HOME"), wxConvFile ), wxT("saga_gui"), wxT("sg-project"));
	CSG_String	sHome(getenv("HOME"));
	wxFileName	fProject(sHome.c_str(), "saga_gui", "sg-project");
#else
	wxFileName	fProject(g_pSAGA->Get_App_Path(), "saga_gui", "sg-project");

	if(	( fProject.FileExists() && (!fProject.IsFileReadable() || !fProject.IsFileWritable()))
	||	(!fProject.FileExists() && (!fProject.IsDirReadable () || !fProject.IsDirWritable ())) )
	{
		fProject.Assign(wxGetHomeDir(), "saga_gui", "sg-project");
	}
#endif

	fProject.Normalize(wxPATH_NORM_ENV_VARS|wxPATH_NORM_DOTS|wxPATH_NORM_TILDE|wxPATH_NORM_ABSOLUTE|wxPATH_NORM_LONG|wxPATH_NORM_SHORTCUT);

	return( fProject );
}

//---------------------------------------------------------
bool CWKSP_Data_Manager::Initialise(void)
{
	//-----------------------------------------------------
	if( m_pProject->Has_File_Name() ) { return( m_pProject->Load(false) ); }
	//-----------------------------------------------------

	//-----------------------------------------------------
	wxFileName fProject, fLastState = Get_SAGA_GUI_CFG();

	switch( m_Parameters("PROJECT_START")->asInt() )
	{
	case 1:	// last state
		fProject = fLastState;
		break;

	case 2:	// ask what to do
		{
			wxArrayString Projects;

			Projects.Add(wxString::Format("[%s]", _TL("empty")));

			if( fLastState.FileExists() && g_pTools->Get_Parameter("SAVE_CONFIG")->asBool() )
			{
				Projects.Add(wxString::Format("[%s]", _TL("last state")));
			}

			m_pMenu_Files->Recent_Get(SG_DATAOBJECT_TYPE_Undefined, Projects, true);

			if( Projects.Count() > 1 )
			{
				wxSingleChoiceDialog dlg(MDI_Get_Top_Window(), _TL("Startup Project"), _TL("Select Startup Project"), Projects);

				if( dlg.ShowModal() == wxID_OK && dlg.GetSelection() > 0 )
				{
					if( fLastState.FileExists() && dlg.GetSelection() == 1 )
					{	// last state
						fProject = fLastState;
					}
					else
					{	// recently opened project
						fProject = dlg.GetStringSelection();
					}
				}
			}
		}
		break;
	}

	//-----------------------------------------------------
	if( fProject.FileExists() )
	{
		return( m_pProject->Load(fProject.GetFullPath(), false, fProject != fLastState) );
	}

	return( true );	// empty
}

//---------------------------------------------------------
bool CWKSP_Data_Manager::Finalise(void)
{
	wxFileName fProject = Get_SAGA_GUI_CFG();

	if( Get_Count() > 0 && g_pTools->Get_Parameter("SAVE_CONFIG")->asBool() )
	{	// last state
		m_pProject->Save(fProject.GetFullPath(), false);

		if( fProject.GetPath().Find(g_pSAGA->Get_App_Path()) == 0 )
		{
			fProject.MakeRelativeTo(g_pSAGA->Get_App_Path());
		}
	}
	else if( fProject.FileExists() )
	{
		wxRemoveFile(fProject.GetFullPath());
	}

	return( true );
}


///////////////////////////////////////////////////////////
//                                                       //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
wxString CWKSP_Data_Manager::Get_Name(void)
{
	return( _TL("Data") );
}

//---------------------------------------------------------
wxString CWKSP_Data_Manager::Get_Description(void)
{
	wxString	s;

	//-----------------------------------------------------
	s	+= wxString::Format("<h4>%s</h4>", _TL("Data"));

	s	+= "<table border=\"0\">";

	DESC_ADD_INT(_TL("Number of Data Sets"), Get_Count());

	if( m_pProject->Has_File_Name() )
	{
		DESC_ADD_STR(_TL("Project File"), m_pProject->Get_File_Name());
	}

	DESC_ADD_INT(_TL("Tables"      ), Get_Tables     () ? Get_Tables     ()->Get_Count      () : 0);
	DESC_ADD_INT(_TL("Shapes"      ), Get_Shapes     () ? Get_Shapes     ()->Get_Items_Count() : 0);
	DESC_ADD_INT(_TL("TIN"         ), Get_TINs       () ? Get_TINs       ()->Get_Count      () : 0);
	DESC_ADD_INT(_TL("Point Clouds"), Get_PointClouds() ? Get_PointClouds()->Get_Count      () : 0);
	DESC_ADD_INT(_TL("Grid Systems"), Get_Grids      () ? Get_Grids      ()->Get_Count      () : 0);
	DESC_ADD_INT(_TL("Grids"       ), Get_Grids      () ? Get_Grids      ()->Get_Items_Count() : 0);

	s	+= wxT("</table>");

	return( s );
}

//---------------------------------------------------------
wxMenu * CWKSP_Data_Manager::Get_Menu(void)
{
	if( m_Sel_Items.Count() > 0 )
	{
		wxMenu	*pMenu	= new wxMenu;

		CMD_Menu_Add_Item(pMenu, false, ID_CMD_WKSP_ITEM_SHOW);
		CMD_Menu_Add_Item(pMenu, false, ID_CMD_WKSP_ITEM_CLOSE);
		pMenu->AppendSeparator();
		CMD_Menu_Add_Item(pMenu, false, ID_CMD_DATA_SETTINGS_LOAD);
		CMD_Menu_Add_Item(pMenu, false, ID_CMD_DATA_SETTINGS_COPY);
		CMD_Menu_Add_Item(pMenu, false, ID_CMD_DATA_FORCE_UPDATE);

		return( pMenu );
	}

	//-----------------------------------------------------
	wxMenu	*pMenu	= new wxMenu(_TL("Data"));

	if( wxGetKeyState(WXK_CONTROL) ) // add advanced/hidden commands
	{
		wxMenu *pAdvanced = new wxMenu;

		CMD_Menu_Add_Item(pAdvanced, false, ID_CMD_DATA_MANAGER_LIST);

		pMenu->AppendSubMenu(pAdvanced, _TL("Advanced"));
		pMenu->AppendSeparator();
	}

	//-----------------------------------------------------
	CMD_Menu_Add_Item(pMenu, false, ID_CMD_DATA_PROJECT_OPEN);
//	CMD_Menu_Add_Item(pMenu, false, ID_CMD_DATA_PROJECT_OPEN_ADD);
	CMD_Menu_Add_Item(pMenu, false, ID_CMD_DATA_PROJECT_NEW);

	if( Get_Count() > 0 )
	{
		CMD_Menu_Add_Item(pMenu, false, ID_CMD_DATA_PROJECT_SAVE);
		CMD_Menu_Add_Item(pMenu, false, ID_CMD_DATA_PROJECT_SAVE_AS);
		CMD_Menu_Add_Item(pMenu, false, ID_CMD_DATA_PROJECT_COPY);
	//	CMD_Menu_Add_Item(pMenu, false, ID_CMD_DATA_PROJECT_COPY_DB);
		pMenu->AppendSeparator();
		CMD_Menu_Add_Item(pMenu, false, ID_CMD_WKSP_ITEM_SEARCH);
	}

	//-----------------------------------------------------
	return( pMenu );
}


///////////////////////////////////////////////////////////
//                                                       //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CWKSP_Data_Manager::On_Command(int Cmd_ID)
{
	if( Open_CMD(Cmd_ID) )
	{
		return( true );
	}

	if( m_pTables && Cmd_ID >= ID_CMD_TABLE_FIRST  && Cmd_ID <= ID_CMD_TABLE_LAST  && m_pTables->On_Command(Cmd_ID) )
	{
		return( true );
	}

	if( m_pShapes && Cmd_ID >= ID_CMD_SHAPES_FIRST && Cmd_ID <= ID_CMD_SHAPES_LAST && m_pShapes->On_Command(Cmd_ID) )
	{
		return( true );
	}

	if( m_pTINs   && Cmd_ID >= ID_CMD_TIN_FIRST    && Cmd_ID <= ID_CMD_TIN_LAST    && m_pTINs  ->On_Command(Cmd_ID) )
	{
		return( true );
	}

	if( m_pPointClouds && Cmd_ID >= ID_CMD_POINTCLOUD_FIRST && Cmd_ID <= ID_CMD_POINTCLOUD_LAST && m_pPointClouds->On_Command(Cmd_ID) )
	{
		return( true );
	}

	if( m_pGrids  && Cmd_ID >= ID_CMD_GRID_FIRST   && Cmd_ID <= ID_CMD_GRID_LAST   && m_pGrids ->On_Command(Cmd_ID) )
	{
		return( true );
	}

	if( m_pGrids  && Cmd_ID >= ID_CMD_GRIDS_FIRST  && Cmd_ID <= ID_CMD_GRIDS_LAST  && m_pGrids ->On_Command(Cmd_ID) )
	{
		return( true );
	}

	//-----------------------------------------------------
	switch( Cmd_ID )
	{
	default:
		return( CWKSP_Base_Manager::On_Command(Cmd_ID) );

	//-----------------------------------------------------
	case ID_CMD_WKSP_ITEM_RETURN     :                                      break;
	case ID_CMD_WKSP_ITEM_CLOSE      : Close(false);                        break;

	//-----------------------------------------------------
	case ID_CMD_DATA_PROJECT_OPEN    : m_pProject->Load(false);             break;
	case ID_CMD_DATA_PROJECT_OPEN_ADD: m_pProject->Load( true);             break;
	case ID_CMD_DATA_PROJECT_BROWSE  : Open_Browser();                      break;
	case ID_CMD_DATA_PROJECT_NEW     : Close(false);                        break;
	case ID_CMD_DATA_PROJECT_SAVE    : m_pProject->Save(true);              break;
	case ID_CMD_DATA_PROJECT_SAVE_AS : m_pProject->Save();                  break;
	case ID_CMD_DATA_PROJECT_COPY    : m_pProject->Copy();                  break;
	case ID_CMD_DATA_PROJECT_COPY_DB : m_pProject->CopyToDB();              break;

	//-----------------------------------------------------
	case ID_CMD_TABLE_OPEN           : Open(SG_DATAOBJECT_TYPE_Table     ); break;
	case ID_CMD_SHAPES_OPEN          : Open(SG_DATAOBJECT_TYPE_Shapes    ); break;
	case ID_CMD_TIN_OPEN             : Open(SG_DATAOBJECT_TYPE_TIN       ); break;
	case ID_CMD_POINTCLOUD_OPEN      : Open(SG_DATAOBJECT_TYPE_PointCloud); break;
	case ID_CMD_GRID_OPEN            : Open(SG_DATAOBJECT_TYPE_Grid      ); break;
	case ID_CMD_GRIDS_OPEN           : Open(SG_DATAOBJECT_TYPE_Grids     ); break;

	//-----------------------------------------------------
	case ID_CMD_DATA_MANAGER_LIST    : {
		CSG_String s(SG_Get_Data_Manager().Get_Summary());
		MSG_General_Add_Line();
		MSG_General_Add(s.c_str());
		MSG_General_Add_Line();
		break; }

	//-----------------------------------------------------
	case ID_CMD_DATA_FORCE_UPDATE: {
		{
			for(size_t i=0; i<MultiSelect_Count(); i++)
			{
				((CWKSP_Data_Item *)Get_Control()->GetItemData(m_Sel_Items[i]))->Force_Update();
			}
		}
		break; }

	case ID_CMD_DATA_SETTINGS_LOAD: {
		wxString File; CSG_MetaData Data;

		if( DLG_Open(File, ID_DLG_PARAMETERS_OPEN) && Data.Load(&File) )
		{
			for(size_t i=0; i<MultiSelect_Count(); i++)
			{
				((CWKSP_Data_Item *)Get_Control()->GetItemData(m_Sel_Items[i]))->Load_Settings(Data);
			}
		}
		break; }

	case ID_CMD_DATA_SETTINGS_COPY: {
		CSG_Parameters *pParameters = Get_Settings_Dialog();

		if( pParameters )
		{
			for(size_t i=0; i<MultiSelect_Count(); i++)
			{
				((CWKSP_Data_Item *)Get_Control()->GetItemData(m_Sel_Items[i]))->Copy_Settings(pParameters);
			}
		}
		break; }
	}

	//-----------------------------------------------------
	return( true );
}

//---------------------------------------------------------
bool CWKSP_Data_Manager::On_Command_UI(wxUpdateUIEvent &event)
{
	switch( event.GetId() )
	{
	default:
		return( CWKSP_Base_Manager::On_Command_UI(event) );

	case ID_CMD_WKSP_ITEM_CLOSE:
		event.Enable(Get_Count() > 0 && g_pTool == NULL);
		break;

	case ID_CMD_DATA_PROJECT_SAVE:
		event.Enable(Get_Count() > 0 && m_pProject->Has_File_Name() );
		break;

	case ID_CMD_DATA_PROJECT_SAVE_AS:
		event.Enable(Get_Count() > 0);
		break;
	}

	return( true );
}


///////////////////////////////////////////////////////////
//                                                       //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
CSG_Parameters * CWKSP_Data_Manager::Get_Parameters(void)
{
	if( m_Sel_Parms[0].Get_Count() > 0 )
	{
		return( &m_Sel_Parms[0] );
	}

	return( m_Parameters.Get_Count() > 0 ? &m_Parameters : NULL );
}

//---------------------------------------------------------
void CWKSP_Data_Manager::Parameters_Changed(void)
{
	if( MultiSelect_Update() )
	{
		return;
	}

	SG_DataObject_Set_Max_Samples    (m_Parameters("DATA_SAMPLE_MAX"     )->asInt   ());

	SG_Grid_Cache_Set_Mode           (m_Parameters("GRID_CACHE_MODE"     )->asInt   ());
	SG_Grid_Cache_Set_Threshold_MB   (m_Parameters("GRID_CACHE_THRSHLD"  )->asDouble());
	SG_Grid_Cache_Set_Directory      (m_Parameters("GRID_CACHE_TMPDIR"   )->asString());

	CSG_Grid_System::Set_Precision   (m_Parameters("GRID_COORD_PRECISION")->asInt   ());

	SG_Set_History_Depth             (m_Parameters("HISTORY_DEPTH"       )->asInt   ());
	SG_Set_History_Ignore_Lists      (m_Parameters("HISTORY_LISTS"       )->asInt   ());

	switch( m_Parameters("GRID_FMT_DEFAULT")->asInt() )
	{
	default: SG_Grid_Set_File_Format_Default(GRID_FILE_FORMAT_Compressed); break;
	case  1: SG_Grid_Set_File_Format_Default(GRID_FILE_FORMAT_Binary    ); break;
	case  2: SG_Grid_Set_File_Format_Default(GRID_FILE_FORMAT_GeoTIFF   ); break;
	}

	switch( m_Parameters("SHAPES_FMT_DEFAULT")->asInt() )
	{
	default: SG_Shapes_Set_File_Format_Default(SHAPE_FILE_FORMAT_ESRI      ); break;
	case  1: SG_Shapes_Set_File_Format_Default(SHAPE_FILE_FORMAT_GeoPackage); break;
	case  2: SG_Shapes_Set_File_Format_Default(SHAPE_FILE_FORMAT_GeoJSON   ); break;
	}

	if( g_pData_Buttons ) { g_pData_Buttons->Update_Buttons(); }

	CWKSP_Base_Manager::Parameters_Changed();
}


///////////////////////////////////////////////////////////
//                                                       //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
int CWKSP_Data_Manager::On_Parameter_Changed(CSG_Parameters *pParameters, CSG_Parameter *pParameter, int Flags)
{
	//-----------------------------------------------------
	if( Flags & PARAMETER_CHECK_VALUES )
	{
		if( g_pSAGA_Frame && g_pData )
		{
			if( pParameter->Cmp_Identifier("THUMBNAILS") )
			{
				if( DLG_Message_Confirm(_TL("Close now ?"), _TL("Restart SAGA to apply the changes")) )
				{
					m_Parameters.Assign_Values(pParameters);

					if( g_pData->Close(true) )
					{
						g_pSAGA_Frame->Close();
					}
				}
			}
		}
	}

	//-----------------------------------------------------
	if( Flags & PARAMETER_CHECK_ENABLE )
	{
		if(	pParameter->Cmp_Identifier("THUMBNAILS") )
		{
			pParameter->Set_Children_Enabled(pParameter->asBool());
		}

		if(	pParameter->Cmp_Identifier("GRID_CACHE_MODE") )
		{
			pParameters->Set_Enabled("GRID_CACHE_THRSHLD", pParameter->asInt() != 0);
			pParameters->Set_Enabled("GRID_CACHE_TMPDIR" , pParameter->asInt() != 0);
		}

		if(	pParameter->Cmp_Identifier("TABLE_FLT_STYLE") )
		{
			pParameters->Set_Enabled("TABLE_FLT_DECIMALS", pParameter->asInt() == 2);
		}
	}

	//-----------------------------------------------------
	return( CWKSP_Base_Manager::On_Parameter_Changed(pParameters, pParameter, Flags) );
}


///////////////////////////////////////////////////////////
//                                                       //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
CWKSP_Data_Item * CWKSP_Data_Manager::Open(const wxString &File, int DataType)
{
	CSG_Data_Object *pObject = NULL;

	switch( DataType )
	{
	case SG_DATAOBJECT_TYPE_Table     : pObject = SG_Create_Table     (&File); break;
	case SG_DATAOBJECT_TYPE_Shapes    : pObject = SG_Create_Shapes    (&File); break;
	case SG_DATAOBJECT_TYPE_TIN       : pObject = SG_Create_TIN       (&File); break;
	case SG_DATAOBJECT_TYPE_PointCloud: pObject = SG_Create_PointCloud(&File); break;
	case SG_DATAOBJECT_TYPE_Grid      : pObject = SG_Create_Grid      (&File); break;
	case SG_DATAOBJECT_TYPE_Grids     : pObject = SG_Create_Grids     (&File); break;
	}

	if( pObject )
	{
		CWKSP_Data_Item *pItem = Add(pObject);

		if( pItem )
		{
			m_pMenu_Files->Recent_Add(pObject->Get_ObjectType(), File);

			SG_Get_Data_Manager().Add(pObject);

			return( pItem );
		}

		delete(pObject);
	}

	m_pMenu_Files->Recent_Del(DataType, File);

	return( NULL );
}

//---------------------------------------------------------
bool CWKSP_Data_Manager::Open(const wxString &File)
{
	if( (SG_File_Cmp_Extension(&File, "txt" ) && !File.Right(8).CmpNoCase("_MTL.txt" ))
	||  (SG_File_Cmp_Extension(&File, "xml" ) && !File.Right(8).CmpNoCase("_MTL.xml" ))
	||  (SG_File_Cmp_Extension(&File, "json") && !File.Right(9).CmpNoCase("_MTL.json")) )
	{
		CSG_Tool *pTool = SG_Get_Tool_Library_Manager().Get_Tool("imagery_tools", 14); // Import Landsat Scene

		return(	pTool && pTool->On_Before_Execution() && pTool->Set_Parameter("METAFILE", &File)
			&& DLG_Parameters(pTool->Get_Parameters()) && pTool->Execute()
		);
	}

	//-----------------------------------------------------
	if( SG_File_Cmp_Extension(&File, "xml") && SG_File_Get_Name(&File, false).Find("MTD_MSI") == 0 )
	{
		CSG_Tool *pTool = SG_Get_Tool_Library_Manager().Get_Tool("imagery_tools", 15); // Import Sentinel-2 Scene

		return(	pTool && pTool->On_Before_Execution() && pTool->Set_Parameter("METAFILE", &File)
			&& DLG_Parameters(pTool->Get_Parameters()) && pTool->Execute()
		);
	}

	//-----------------------------------------------------
	if( SG_File_Cmp_Extension(&File, "sg-project")
	||  SG_File_Cmp_Extension(&File, "sprj"      ) )
	{
		return( m_pProject->Load(File, false, true) );
	}

	if( SG_File_Cmp_Extension(&File, "txt"     )
	||	SG_File_Cmp_Extension(&File, "csv"     )
	||	SG_File_Cmp_Extension(&File, "dbf"     ) )
	{
		return( Open(File, SG_DATAOBJECT_TYPE_Table     ) != NULL );
	}

	if( SG_File_Cmp_Extension(&File, "shp"     ) )
	{
		return( Open(File, SG_DATAOBJECT_TYPE_Shapes    ) != NULL );
	}

	if( SG_File_Cmp_Extension(&File, "sg-pts-z")
	||  SG_File_Cmp_Extension(&File, "sg-pts"  )
	||  SG_File_Cmp_Extension(&File, "spc"     ) )
	{
		return( Open(File, SG_DATAOBJECT_TYPE_PointCloud) != NULL );
	}

	if(	SG_File_Cmp_Extension(&File, "sg-grd-z")
	||	SG_File_Cmp_Extension(&File, "sg-grd"  )
	||	SG_File_Cmp_Extension(&File, "sgrd"    )
	||  SG_File_Cmp_Extension(&File, "dgm"     )
	||	SG_File_Cmp_Extension(&File, "grd"     ) )
	{
		return( Open(File, SG_DATAOBJECT_TYPE_Grid      ) != NULL );
	}

	if( SG_File_Cmp_Extension(&File, "sg-gds-z")
	||  SG_File_Cmp_Extension(&File, "sg-gds"  ) )
	{
		return( Open(File, SG_DATAOBJECT_TYPE_Grids     ) != NULL );
	}

	return( SG_Get_Data_Manager().Add(&File) != NULL );
}

//---------------------------------------------------------
bool CWKSP_Data_Manager::Open(int DataType)
{
	int ID;

	switch( DataType )
	{
	case SG_DATAOBJECT_TYPE_Table     : ID = ID_DLG_TABLE_OPEN     ; break;
	case SG_DATAOBJECT_TYPE_Shapes    : ID = ID_DLG_SHAPES_OPEN    ; break;
	case SG_DATAOBJECT_TYPE_TIN       : ID = ID_DLG_TIN_OPEN       ; break;
	case SG_DATAOBJECT_TYPE_PointCloud: ID = ID_DLG_POINTCLOUD_OPEN; break;
	case SG_DATAOBJECT_TYPE_Grid      : ID = ID_DLG_GRID_OPEN      ; break;
	case SG_DATAOBJECT_TYPE_Grids     : ID = ID_DLG_GRIDS_OPEN     ; break;
	default                           : return( false );
	}

	//-----------------------------------------------------
	bool bResult = false; wxArrayString Files;

	if( DLG_Open(Files, ID) )
	{
		MSG_General_Add_Line();

		for(size_t i=0; i<Files.GetCount(); i++)
		{
			if( Open(Files[i], DataType) )
			{
				bResult = true;
			}
		}
	}

	return( bResult );
}

//---------------------------------------------------------
bool CWKSP_Data_Manager::Open_CMD(int Cmd_ID)
{
	return( m_pMenu_Files->Recent_Open(Cmd_ID) );
}


///////////////////////////////////////////////////////////
//                                                       //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CWKSP_Data_Manager::Open_Browser(void)
{
	//-----------------------------------------------------
	wxString	Directory	= wxDirSelector(_TL("Search for Projects"));

	if( Directory.IsEmpty() )
	{
		return( false );
	}

	//-----------------------------------------------------
	wxArrayString	Projects;

	Open_Browser(Projects, Directory);

	if( Projects.Count() == 0 )
	{
		wxMessageBox(_TL("No projects in directory"), _TL("Search for Projects"), wxOK|wxICON_EXCLAMATION);

		return( false );
	}

	//-----------------------------------------------------
	wxSingleChoiceDialog	dlg(MDI_Get_Top_Window(), _TL("Open Project"), _TL("Search for Projects"), Projects);

	return( dlg.ShowModal() == wxID_OK && Open(dlg.GetStringSelection()) );
}

//---------------------------------------------------------
#include <wx/dir.h>

//---------------------------------------------------------
bool CWKSP_Data_Manager::Open_Browser(wxArrayString &Projects, const wxString &Directory)
{
	wxDir	Dir;

	if( Dir.Open(Directory) )
	{
		wxString	FileName;

		if( Dir.GetFirst(&FileName, wxEmptyString, wxDIR_FILES) )
		{
			do
			{
				wxFileName	fn(Dir.GetName(), FileName);

				if( !fn.GetExt().CmpNoCase("sprj") )
				{
					Projects.Add(fn.GetFullPath());
				}
			}
			while( Dir.GetNext(&FileName) );
		}

		if( Dir.GetFirst(&FileName, wxEmptyString, wxDIR_DIRS) )
		{
			do
			{
				wxFileName	fn(Dir.GetName(), FileName);

				Open_Browser(Projects, fn.GetFullPath());
			}
			while( Dir.GetNext(&FileName) );
		}
	}

	return( Projects.Count() > 0 );
}


///////////////////////////////////////////////////////////
//                                                       //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CWKSP_Data_Manager::Save_Modified(CWKSP_Base_Item *pItem, bool bSelections)
{
	CSG_Parameters	Parameters(_TL("Save Modified Data"));

	Parameters.Add_Bool("", "SAVE_ALL", _TL("Save all"), _TL(""), false);

	wxFileName	Directory(m_pProject->Get_File_Name());

	if( !Directory.DirExists() )
	{
		Directory.AssignDir(wxFileName::GetTempDir() + wxFileName::GetPathSeparator() + "saga");

		Directory.Mkdir(wxS_DIR_DEFAULT, wxPATH_MKDIR_FULL);
	}

	_Modified_Get(&Parameters, pItem ? pItem : this, Directory.GetPath(), bSelections);

	if( Parameters.Get_Count() > 1 )
	{
		Parameters.Set_Callback_On_Parameter_Changed(&_Modified_Changed);

		if( !DLG_Parameters(&Parameters) )
		{
			return( false );
		}

		_Modified_Save(&Parameters);
	}

	return( true );
}

//---------------------------------------------------------
int CWKSP_Data_Manager::_Modified_Changed(CSG_Parameter *pParameter, int Flags)
{
	CSG_Parameters	*pParameters	= pParameter ? pParameter->Get_Parameters() : NULL;

	if( pParameters )
	{
		if( pParameter->Cmp_Identifier("SAVE_ALL") )
		{
			for(int i=0; i<pParameters->Get_Count(); i++)
			{
				CSG_Parameter	*pFile	= pParameters->Get_Parameter(i);

				if( pFile->Get_Type() == PARAMETER_TYPE_Bool )
				{
					pFile->Set_Value(pParameter->asBool());

					for(int j=0; j<pFile->Get_Children_Count(); j++)
					{
						pFile->Get_Child(j)->Set_Enabled(pParameter->asBool());
					}
				}
			}
		}
		else if( pParameter->Get_Type() == PARAMETER_TYPE_Bool )
		{
			if( !pParameter->asBool() && pParameters->Get_Parameter("SAVE_ALL") )
			{
				pParameters->Get_Parameter("SAVE_ALL")->Set_Value(0);
			}

			for(int j=0; j<pParameter->Get_Children_Count(); j++)
			{
				pParameter->Get_Child(j)->Set_Enabled(pParameter->asBool());
			}
		}

		return( 1 );
	}

	return( 0 );
}

//---------------------------------------------------------
bool CWKSP_Data_Manager::_Modified_Get(CSG_Parameters *pParameters, CWKSP_Base_Item *pItem, const wxString &Directory, bool bSelections)
{
	if( pItem && pParameters )
	{
		switch( pItem->Get_Type() )
		{
		default:
			break;

		//-------------------------------------------------
		case WKSP_ITEM_Data_Manager      :
		case WKSP_ITEM_Table_Manager     :
		case WKSP_ITEM_Shapes_Manager    :
		case WKSP_ITEM_Shapes_Type       :
		case WKSP_ITEM_TIN_Manager       :
		case WKSP_ITEM_PointCloud_Manager:
		case WKSP_ITEM_Grid_Manager      :
		case WKSP_ITEM_Grid_System       :
			for(int i=0; i<((CWKSP_Base_Manager *)pItem)->Get_Count(); i++)
			{
				_Modified_Get(pParameters, ((CWKSP_Base_Manager *)pItem)->Get_Item(i), Directory, bSelections && !pItem->is_Selected());
			}
			break;

		//-------------------------------------------------
		case WKSP_ITEM_Table:
			if( !bSelections || pItem->is_Selected() )
			{
				_Modified_Get(pParameters, pItem, Directory, ((CWKSP_Table *)pItem)->Get_Table() );
			}
			break;

		case WKSP_ITEM_Shapes    :
		case WKSP_ITEM_TIN       :
		case WKSP_ITEM_PointCloud:
		case WKSP_ITEM_Grid      :
		case WKSP_ITEM_Grids     :
			if( !bSelections || pItem->is_Selected() )
			{
				_Modified_Get(pParameters, pItem, Directory, ((CWKSP_Layer *)pItem)->Get_Object());
			}
			break;
		}
	}

	return( true );
}

//---------------------------------------------------------
bool CWKSP_Data_Manager::_Modified_Get(CSG_Parameters *pParameters, CWKSP_Base_Item *pItem, const wxString &Directory, CSG_Data_Object *pObject)
{
	if( !pObject->is_Modified() )
	{
		return( false );
	}

	//-----------------------------------------------------
	wxString Filter, Extension;

	switch( pItem->Get_Type() )
	{
	case WKSP_ITEM_Table     : Filter = DLG_Get_FILE_Filter(ID_DLG_TABLE_SAVE     ); Extension = "txt"     ; break;
	case WKSP_ITEM_Shapes    : Filter = DLG_Get_FILE_Filter(ID_DLG_SHAPES_SAVE    ); Extension = SG_Shapes_Get_File_Extension_Default().c_str(); break;
	case WKSP_ITEM_TIN       : Filter = DLG_Get_FILE_Filter(ID_DLG_TIN_SAVE       ); Extension = SG_Shapes_Get_File_Extension_Default().c_str(); break;
	case WKSP_ITEM_PointCloud: Filter = DLG_Get_FILE_Filter(ID_DLG_POINTCLOUD_SAVE); Extension = "sg-pts-z"; break;
	case WKSP_ITEM_Grid      : Filter = DLG_Get_FILE_Filter(ID_DLG_GRID_SAVE      ); Extension = SG_Grid_Get_File_Extension_Default().c_str(); break;
	case WKSP_ITEM_Grids     : Filter = DLG_Get_FILE_Filter(ID_DLG_GRIDS_SAVE     ); Extension = "sg-gds-z"; break;
	default: return( false );
	}

	//-----------------------------------------------------
	wxFileName Path(pObject->Get_File_Name());

	if( !Path.FileExists() )
	{
		wxString Name(pObject->Get_Name());

		Name.Replace("." , "_");
		Name.Replace(":" , "_");
		Name.Replace("/" , "_");
		Name.Replace("\\", "_");

		Path.SetPath(Directory);
		Path.SetExt (Extension);

		for(int i=0, bOkay=false; !bOkay; i++)
		{
			if( i == 0 )
			{
				Path.SetName(Name);
			}
			else
			{
				Path.SetName(Name + wxString::Format("_%d", i));
			}

			bOkay = !Path.FileExists();

			for(int j=0; bOkay && j<pParameters->Get_Count(); j++)
			{
				CSG_Parameter &P = *pParameters->Get_Parameter(j);

				if( P.Get_Type() == PARAMETER_TYPE_FilePath && Path == P.asString() )
				{
					bOkay = false;
				}
			}
		}
	}

	//-----------------------------------------------------
	CSG_String	Manager, Object;

	Manager.Printf("%p", pItem->Get_Manager());

	if( pParameters->Get_Parameter(Manager) == NULL )
	{
		pParameters->Add_Node("", Manager, pItem->Get_Manager()->Get_Name().wx_str(), "");
	}			

	Object.Printf("%p", pObject);

	pParameters->Add_Bool(Manager, Object, pItem->Get_Name().wx_str(), "", false);
	pParameters->Add_FilePath(Object, Object + "FILE", _TL("File"), "", Filter, Path.GetFullPath(), true);

	return( true );
}

//---------------------------------------------------------
bool CWKSP_Data_Manager::_Modified_Save(CSG_Parameters *pParameters)
{
	for(int i=0; i<pParameters->Get_Count(); i++)
	{
		void			*Pointer;
		CSG_Data_Object	*pObject;
		CSG_Parameter	*pParameter	= pParameters->Get_Parameter(i);

		if(	pParameter->Get_Type() == PARAMETER_TYPE_Bool && pParameter->asBool()
		&&  SG_SSCANF(pParameter->Get_Identifier(), SG_T("%p"), &Pointer) == 1
		&&  SG_Get_Data_Manager().Exists(pObject = (CSG_Data_Object *)Pointer) )
		{
			pParameter	= pParameters->Get_Parameter(CSG_String::Format("%pFILE", pObject));

			if(	pParameter && pParameter->asString() && pParameter->asString()[0] )
			{
				pObject->Save(pParameter->asString());
			}
		}
	}

	return( true );
}

//---------------------------------------------------------
bool CWKSP_Data_Manager::Save_Modified_Sel(void)
{
	return( Save_Modified(this, true) );
}

//---------------------------------------------------------
bool CWKSP_Data_Manager::Close(bool bSilent)
{
	if( g_pTool )
	{
		if( !g_pTool->is_Executing() )
		{
			g_pTool->Finish(false, false);
		}
		else
		{
			if( bSilent )
			{
				MSG_General_Add(_TL("Stop running tool or wait until its execution has been finished!"), true, false);
			}
			else
			{
				DLG_Message_Show(_TL("Stop running tool or wait until its execution has been finished!"), g_pTool->Get_Name());
			}

			return( false );
		}
	}

	if( Get_Count() == 0 || ((bSilent || DLG_Message_Confirm(_TL("Close all data sets"), _TL("Close"))) && Save_Modified(this)) )
	{
		m_pProject->Clr_File_Name();

		g_pActive->Get_Parameters()->Restore_Parameters();

		g_pSAGA_Frame->Close_Children();

		g_pMaps->Close(true);

		g_pData_Ctrl->Close(true);

		return( true );
	}

	return( false );
}


///////////////////////////////////////////////////////////
//                                                       //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
CWKSP_Base_Manager * CWKSP_Data_Manager::Get_Manager(TSG_Data_Object_Type Type, bool bAdd)
{
	#define GET_MANAGER(pManager, Class)	if( !pManager && bAdd ) Add_Item(pManager = new Class); return( pManager );

	switch( Type )
	{
	case SG_DATAOBJECT_TYPE_Table     : GET_MANAGER(m_pTables     , CWKSP_Table_Manager     );
	case SG_DATAOBJECT_TYPE_TIN       : GET_MANAGER(m_pTINs       , CWKSP_TIN_Manager       );
	case SG_DATAOBJECT_TYPE_PointCloud: GET_MANAGER(m_pPointClouds, CWKSP_PointCloud_Manager);
	case SG_DATAOBJECT_TYPE_Shapes    : GET_MANAGER(m_pShapes     , CWKSP_Shapes_Manager    );
	case SG_DATAOBJECT_TYPE_Grid      : GET_MANAGER(m_pGrids      , CWKSP_Grid_Manager      );
	case SG_DATAOBJECT_TYPE_Grids     : GET_MANAGER(m_pGrids      , CWKSP_Grid_Manager      );
	default:	break;
	}

	return( NULL );
}

//---------------------------------------------------------
void CWKSP_Data_Manager::Del_Manager(CWKSP_Base_Item *pItem)
{
	if( pItem == m_pTables      ) { m_pTables      = NULL; }
	if( pItem == m_pTINs        ) { m_pTINs        = NULL; }
	if( pItem == m_pPointClouds ) { m_pPointClouds = NULL; }
	if( pItem == m_pShapes      ) { m_pShapes      = NULL; }
	if( pItem == m_pGrids       ) { m_pGrids       = NULL; }

	if( Get_Count() == 0 )
	{
		m_pProject->Clr_File_Name();
	}
}


///////////////////////////////////////////////////////////
//                                                       //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
CWKSP_Data_Item * CWKSP_Data_Manager::Add(CSG_Data_Object *pObject)
{
	if( SG_Get_Data_Manager().Add(pObject) && Get_Manager(pObject->Get_ObjectType(), true) )
	{
		switch( pObject->Get_ObjectType() )
		{
		case SG_DATAOBJECT_TYPE_Table     : return( (CWKSP_Data_Item *)m_pTables     ->Add_Data((CSG_Table      *)pObject) );
		case SG_DATAOBJECT_TYPE_TIN       : return( (CWKSP_Data_Item *)m_pTINs       ->Add_Data((CSG_TIN        *)pObject) );
		case SG_DATAOBJECT_TYPE_PointCloud: return( (CWKSP_Data_Item *)m_pPointClouds->Add_Data((CSG_PointCloud *)pObject) );
		case SG_DATAOBJECT_TYPE_Shapes    : return( (CWKSP_Data_Item *)m_pShapes     ->Add_Data((CSG_Shapes     *)pObject) );
		case SG_DATAOBJECT_TYPE_Grid      : return( (CWKSP_Data_Item *)m_pGrids      ->Add_Data((CSG_Grid       *)pObject) );
		case SG_DATAOBJECT_TYPE_Grids     : return( (CWKSP_Data_Item *)m_pGrids      ->Add_Data((CSG_Grids      *)pObject) );
		default: break;
		}
	}

	return( NULL );
}

//---------------------------------------------------------
CWKSP_Data_Item * CWKSP_Data_Manager::Get(CSG_Data_Object *pObject)
{
	if( pObject && pObject != DATAOBJECT_CREATE && Get_Manager(pObject->Get_ObjectType()) )
	{
		switch( pObject->Get_ObjectType() )
		{
		case SG_DATAOBJECT_TYPE_Table     : return( (CWKSP_Data_Item *)m_pTables     ->Get_Data((CSG_Table      *)pObject) );
		case SG_DATAOBJECT_TYPE_TIN       : return( (CWKSP_Data_Item *)m_pTINs       ->Get_Data((CSG_TIN        *)pObject) );
		case SG_DATAOBJECT_TYPE_PointCloud: return( (CWKSP_Data_Item *)m_pPointClouds->Get_Data((CSG_PointCloud *)pObject) );
		case SG_DATAOBJECT_TYPE_Shapes    : return( (CWKSP_Data_Item *)m_pShapes     ->Get_Data((CSG_Shapes     *)pObject) );
		case SG_DATAOBJECT_TYPE_Grid      : return( (CWKSP_Data_Item *)m_pGrids      ->Get_Data((CSG_Grid       *)pObject) );
		case SG_DATAOBJECT_TYPE_Grids     : return( (CWKSP_Data_Item *)m_pGrids      ->Get_Data((CSG_Grids      *)pObject) );
		default: break;
		}
	}

	return( NULL );
}

//---------------------------------------------------------
CWKSP_Layer * CWKSP_Data_Manager::Get_Layer(CSG_Data_Object *pObject)
{
	if( pObject && pObject != DATAOBJECT_CREATE && Get_Manager(pObject->Get_ObjectType()) )
	{
		switch( pObject->Get_ObjectType() )
		{
		case SG_DATAOBJECT_TYPE_TIN       : return( (CWKSP_Layer *)m_pTINs       ->Get_Data((CSG_TIN        *)pObject) );
		case SG_DATAOBJECT_TYPE_PointCloud: return( (CWKSP_Layer *)m_pPointClouds->Get_Data((CSG_PointCloud *)pObject) );
		case SG_DATAOBJECT_TYPE_Shapes    : return( (CWKSP_Layer *)m_pShapes     ->Get_Data((CSG_Shapes     *)pObject) );
		case SG_DATAOBJECT_TYPE_Grid      : return( (CWKSP_Layer *)m_pGrids      ->Get_Data((CSG_Grid       *)pObject) );
		case SG_DATAOBJECT_TYPE_Grids     : return( (CWKSP_Layer *)m_pGrids      ->Get_Data((CSG_Grids      *)pObject) );
		default: break;
		}
	}

	return( NULL );
}

//---------------------------------------------------------
CWKSP_Data_Item * CWKSP_Data_Manager::Get_byFile(const wxString &File)
{
	return( Get(SG_Get_Data_Manager().Find(&File, false)) );
}

//---------------------------------------------------------
CWKSP_Data_Item * CWKSP_Data_Manager::Get_byID(const wxString &ID)
{
	CWKSP_Data_Item *pItem;

	if( m_pTables      && (pItem = (CWKSP_Data_Item *)m_pTables     ->Get_Item_byID(ID)) != NULL ) { return( pItem ); }
	if( m_pTINs        && (pItem = (CWKSP_Data_Item *)m_pTINs       ->Get_Item_byID(ID)) != NULL ) { return( pItem ); }
	if( m_pPointClouds && (pItem = (CWKSP_Data_Item *)m_pPointClouds->Get_Item_byID(ID)) != NULL ) { return( pItem ); }
	if( m_pShapes      && (pItem = (CWKSP_Data_Item *)m_pShapes     ->Get_Item_byID(ID)) != NULL ) { return( pItem ); }
	if( m_pGrids       && (pItem = (CWKSP_Data_Item *)m_pGrids      ->Get_Item_byID(ID)) != NULL ) { return( pItem ); }

	return( NULL );
}

//---------------------------------------------------------
CWKSP_Data_Item * CWKSP_Data_Manager::Get_byID_or_File(const SG_Char *ID, const SG_Char *File, const SG_Char *Root)
{
	CWKSP_Data_Item *pItem = NULL;

	if( ID )
	{
		pItem = Get_byID(ID);
	}

	if( File )
	{
		wxString _File(File);

		if( _File.Find("PGSQL") != 0 && Root && SG_Dir_Exists(Root) )
		{
			_File = Get_FilePath_Absolute(Root, _File);
		}

		if( pItem )
		{
			if( _File.CmpNoCase(pItem->Get_Object()->Get_File_Name(false)) != 0 )
			{
				pItem = NULL;
			}
		}
		else
		{
			pItem = Get_byFile(_File);
		}
	}

	return( pItem );
}

//---------------------------------------------------------
bool CWKSP_Data_Manager::Del(CSG_Data_Object *pObject, bool bConfirm)
{
	return( Get_Control()->Del_Item(Get(pObject), bConfirm == false) ); // bConfirm == true/false => bSilent == false/true
}


///////////////////////////////////////////////////////////
//                                                       //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CWKSP_Data_Manager::Update(CSG_Data_Object *pObject, CSG_Parameters *pParameters)
{
	CWKSP_Data_Item	*pItem	= Get(pObject);

	if( !pItem )
	{
		return( false );
	}

	if( SG_Get_Data_Manager().Exists(pObject) )
	{
		pItem->Get_Parameters()->Assign_Values(pParameters);

		return( pItem->DataObject_Changed() );
	}

	Get_Control()->Del_Item(pItem, true);

	return( true );
}

//---------------------------------------------------------
bool CWKSP_Data_Manager::Update_Views(CSG_Data_Object *pObject)
{
	CWKSP_Data_Item	*pItem	= Get(pObject);

	return( pItem && pItem->Update_Views() );
}

//---------------------------------------------------------
bool CWKSP_Data_Manager::Show(CSG_Data_Object *pObject, int Flags)
{
	CWKSP_Data_Item	*pItem	= Get(pObject);

	return( pItem && pItem->Show(Flags) );
}

//---------------------------------------------------------
bool CWKSP_Data_Manager::asImage(CSG_Data_Object *pObject, CSG_Grid *pImage)
{
	CWKSP_Layer	*pLayer	= Get_Layer(pObject);

	return( pLayer && pLayer->asImage(pImage) );
}

//---------------------------------------------------------
bool CWKSP_Data_Manager::Get_Colors(CSG_Data_Object *pObject, CSG_Colors *pColors)
{
	CWKSP_Layer	*pLayer	= Get_Layer(pObject);

	return( pLayer && pLayer->Get_Colors(pColors) );
}

//---------------------------------------------------------
bool CWKSP_Data_Manager::Set_Colors(CSG_Data_Object *pObject, CSG_Colors *pColors)
{
	if( m_Parameters("COLORS_FROM_TOOL")->asBool() == false )
	{
		return( true );
	}

	CWKSP_Layer	*pLayer	= Get_Layer(pObject);

	return( pLayer && pLayer->Set_Colors(pColors) );
}

//---------------------------------------------------------
bool CWKSP_Data_Manager::Get_Parameters(CSG_Data_Object *pObject, CSG_Parameters *pParameters)
{
	CWKSP_Data_Item	*pItem	= pParameters ? Get(pObject) : NULL;

	return( pItem && pParameters->Assign(pItem->Get_Parameters()) );
}

//---------------------------------------------------------
bool CWKSP_Data_Manager::Set_Parameters(CSG_Data_Object *pObject, CSG_Parameters *pParameters)
{
	CWKSP_Data_Item	*pItem	= pParameters ? Get(pObject) : NULL;

	if( pItem && pItem->Get_Parameters()->Assign_Values(pParameters) )
	{
		pItem->Parameters_Changed();

		return( true );
	}

	return( false );
}

//---------------------------------------------------------
bool CWKSP_Data_Manager::Show_Diagram(CSG_Table *pTable, CSG_Parameters *pParameters)
{
	CWKSP_Data_Item *pItem = Get(pTable);

	switch( pItem->Get_Type() )
	{
	case WKSP_ITEM_Table     : ((CWKSP_Table      *)pItem)->Set_Diagram(true, pParameters); return( true );
	case WKSP_ITEM_Shapes    : ((CWKSP_Shapes     *)pItem)->Set_Diagram(true, pParameters); return( true );
//	case WKSP_ITEM_PointCloud: ((CWKSP_PointCloud *)pItem)->Set_Diagram(true, pParameters); return( true );

	default: break;
	}

	return( false );
}


///////////////////////////////////////////////////////////
//                                                       //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
size_t CWKSP_Data_Manager::MultiSelect_Count(void)
{
	return( m_Sel_Items.Count() );
}

//---------------------------------------------------------
bool CWKSP_Data_Manager::MultiSelect_Check(void)
{
	enum
	{
		TYPE_Table,
		TYPE_Shapes_Point,
		TYPE_Shapes_Points,
		TYPE_Shapes_Line,
		TYPE_Shapes_Polygon,
		TYPE_TIN,
		TYPE_PointCloud,
		TYPE_Grid,
		TYPE_Grids,
		TYPE_Undefined
	};

	//-----------------------------------------------------
	wxArrayTreeItemIds	IDs;

	if( Get_Control()->GetSelections(IDs) > 1 )
	{
		int	iType, Type	= TYPE_Undefined;

		for(size_t iID=0; iID<IDs.Count(); iID++)
		{
			CWKSP_Base_Item	*pItem	= (CWKSP_Base_Item *)Get_Control()->GetItemData(IDs[iID]);

			switch( pItem->Get_Type() )
			{
			default                    :	iType	= TYPE_Undefined     ;	break;
			case WKSP_ITEM_Table       :	iType	= TYPE_Table         ;	break;
			case WKSP_ITEM_TIN         :	iType	= TYPE_TIN           ;	break;
			case WKSP_ITEM_PointCloud  :	iType	= TYPE_PointCloud    ;	break;
			case WKSP_ITEM_Grid        :	iType	= TYPE_Grid          ;	break;
			case WKSP_ITEM_Grids       :	iType	= TYPE_Grids         ;	break;
			case WKSP_ITEM_Shapes      :	switch( ((CWKSP_Shapes *)pItem)->Get_Shapes()->Get_Type() )
				{
				default                :	iType	= TYPE_Undefined     ;	break;
				case SHAPE_TYPE_Point  :	iType	= TYPE_Shapes_Point  ;	break;
				case SHAPE_TYPE_Points :	iType	= TYPE_Shapes_Points ;	break;
				case SHAPE_TYPE_Line   :	iType	= TYPE_Shapes_Line   ;	break;
				case SHAPE_TYPE_Polygon:	iType	= TYPE_Shapes_Polygon;	break;
				}
				break;
			}

			//---------------------------------------------
			if( iType != TYPE_Undefined )
			{
				if( Type == TYPE_Undefined )
				{
					Type	= iType;

					m_Sel_Parms[0].Assign(pItem->Get_Parameters());

					m_Sel_Items.Clear();

					m_Sel_Items.Add(IDs[iID]);
				}
				else if( Type == iType )
				{
					m_Sel_Parms[0].Set_Callback(false);

					m_Sel_Items.Add(IDs[iID]);

					for(int i=0; i<m_Sel_Parms[0].Get_Count(); i++)
					{
						CSG_Parameter	&Parameter	= m_Sel_Parms[0][i];
						
						CSG_Parameter	*pParameter	= pItem->Get_Parameters()->Get_Parameter(Parameter.Get_Identifier());

						if( !Parameter.is_Compatible(pParameter) )
						{
							m_Sel_Parms[0].Del_Parameter(i--);
						}
						else if( !Parameter.is_Value_Equal(pParameter) )
						{
							if( Parameter.is_DataObject() )
							{
								Parameter.Set_Value(DATAOBJECT_NOTSET);
							}
							else if( Parameter.is_DataObject_List() )
							{
								Parameter.asList()->Del_Items();
							}
							else switch( Parameter.Get_Type() )
							{
							default:
								Parameter.Restore_Default();
								break;

							case PARAMETER_TYPE_Bool       :
							case PARAMETER_TYPE_Int        :
							case PARAMETER_TYPE_Double     :
							case PARAMETER_TYPE_Degree     :
							case PARAMETER_TYPE_Color      :
							case PARAMETER_TYPE_Table_Field:
							case PARAMETER_TYPE_Data_Type  :
							case PARAMETER_TYPE_Choice     :	Parameter.Set_Value(0.);	break;

							case PARAMETER_TYPE_Choices    :
							case PARAMETER_TYPE_String     :
							case PARAMETER_TYPE_Text       :
							case PARAMETER_TYPE_FilePath   :	Parameter.Set_Value("");	break;
							}
						}
					}

					m_Sel_Parms[0].Set_Callback(true);
				}
				else //  Type != iType
				{
					IDs.Clear();
				}
			}
		}

		//-------------------------------------------------
		if( IDs.Count() > 0 )
		{
			m_Sel_Parms[1]	= m_Sel_Parms[0];

			if( g_pActive->Get_Active() == this )
			{
				g_pActive->Get_Parameters()->Update_Parameters(&m_Sel_Parms[0], false);
			}

			return( true );
		}
	}

	//-----------------------------------------------------
	m_Sel_Items.Clear();

	m_Sel_Parms[0].Destroy();
	m_Sel_Parms[1].Destroy();

	return( false );
}

//---------------------------------------------------------
bool CWKSP_Data_Manager::MultiSelect_Update(void)
{
	if( m_Sel_Items.Count() == 0 )
	{
		return( false );
	}

	CSG_Parameters	Changed;

	for(int i=0; i<m_Sel_Parms[0].Get_Count(); i++)		// compare to backup list [1] to detect changed parameters
	{
		if( !m_Sel_Parms[0][i].is_Value_Equal(&m_Sel_Parms[1][i]) )
		{
			Changed.Add_Parameter(&m_Sel_Parms[0][i]);
		}
	}

	for(size_t iID=0; iID<m_Sel_Items.Count(); iID++)	// assign changed values to selected data set parameters
	{
		CWKSP_Data_Item	*pItem	= (CWKSP_Data_Item *)Get_Control()->GetItemData(m_Sel_Items[iID]);

		pItem->Get_Parameters()->Assign_Values(&Changed);

		pItem->Parameters_Changed();
	}

	m_Sel_Parms[1].Assign_Values(&m_Sel_Parms[0]);		// update backup list with changed values

	return( true );
}


///////////////////////////////////////////////////////////
//                                                       //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
void CWKSP_Data_Manager::_Get_Settings_Dialog(CSG_Table &List, CWKSP_Base_Item *pItem)
{
	if( pItem )
	{
		if( pItem->is_Manager() )
		{
			for(int i=0; i<((CWKSP_Base_Manager *)pItem)->Get_Count(); i++)
			{
				_Get_Settings_Dialog(List, ((CWKSP_Base_Manager *)pItem)->Get_Item(i));
			}
		}
		else if( pItem->Get_Parameters() )
		{
			CSG_Table_Record	*pEntry	= List.Add_Record();

			pEntry->Set_Value(0, CSG_String(wxString::Format("[%s] %s", pItem->Get_Manager()->Get_Name(), pItem->Get_Name()).wc_str()));
			pEntry->Set_Value(1, CSG_String::Format("%p", pItem->Get_Parameters()));
		}
	}
}

//---------------------------------------------------------
CSG_Parameters * CWKSP_Data_Manager::Get_Settings_Dialog(void)
{
	CSG_Table	List;

	List.Add_Field("NAME", SG_DATATYPE_String);
	List.Add_Field("PRMS", SG_DATATYPE_String);

	_Get_Settings_Dialog(List, g_pData->Get_Grids      ());
	_Get_Settings_Dialog(List, g_pData->Get_Shapes     ());
	_Get_Settings_Dialog(List, g_pData->Get_TINs       ());
	_Get_Settings_Dialog(List, g_pData->Get_PointClouds());

	if( List.Get_Count() > 0 )
	{
		wxArrayString	Items;

		for(int i=0; i<List.Get_Count(); i++)
		{
			Items.Add(List.Get_Record(i)->asString(0));
		}

		wxSingleChoiceDialog	dlg(MDI_Get_Top_Window(),
			_TL("Copy Settings from..."),
			_TL("Select a layer to copy settings from it."),
			Items
		);

		void	*pParameters;

		if( dlg.ShowModal() == wxID_OK && SG_SSCANF(List.Get_Record(dlg.GetSelection())->asString(1), SG_T("%p"), &pParameters) == 1 )
		{
			return( (CSG_Parameters *)pParameters );
		}
	}

	return( NULL );
}


///////////////////////////////////////////////////////////
//                                                       //
//                                                       //
//                                                       //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
