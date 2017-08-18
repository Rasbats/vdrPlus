/******************************************************************************
 *
 * Project:  OpenCPN
 * Purpose:  vdrPlus Plugin
 * Author:   Mike Rossiter
 *
 ***************************************************************************
 *   Copyright (C) 2016 by Mike Rossiter                                *
 *   $EMAIL$                                                               *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************
 */

#include "wx/wxprec.h"

#ifndef  WX_PRECOMP
  #include "wx/wx.h"
#endif //precompiled headers

#include "vdrPlus_pi.h"
#include "vdrPlusgui_impl.h"
#include "vdrPlusgui.h"
#include "ocpn_plugin.h" 
#include "folder.xpm"

class vdrPlus_pi;
class Dlg;

// the class factories, used to create and destroy instances of the PlugIn

extern "C" DECL_EXP opencpn_plugin* create_pi(void *ppimgr)
{
    return new vdrPlus_pi(ppimgr);
}

extern "C" DECL_EXP void destroy_pi(opencpn_plugin* p)
{
    delete p;
}

//---------------------------------------------------------------------------------------------------------
//
//    vdrPlus PlugIn Implementation
//
//---------------------------------------------------------------------------------------------------------

#include "icons.h"

//---------------------------------------------------------------------------------------------------------
//
//          PlugIn initialization and de-init
//
//---------------------------------------------------------------------------------------------------------




vdrPlus_pi::vdrPlus_pi(void *ppimgr)
      :opencpn_plugin_16 (ppimgr)
{
      // Create the PlugIn icons
      initialize_images();
	  m_bShowvdrPlus = false;
}

vdrPlus_pi::~vdrPlus_pi(void)
{
     delete _img_vdrPlus_pi;
     delete _img_vdrPlus;
     
}

int vdrPlus_pi::Init(void)
{
      AddLocaleCatalog( _T("opencpn-vdrPlus_pi") );

      // Set some default private member parameters
      m_hr_dialog_x = 40;
      m_hr_dialog_y = 80;
      ::wxDisplaySize(&m_display_width, &m_display_height);

      //    Get a pointer to the opencpn display canvas, to use as a parent for the POI Manager dialog
      m_parent_window = GetOCPNCanvasWindow();

      //    Get a pointer to the opencpn configuration object
      m_pconfig = GetOCPNConfigObject();

      //    And load the configuration items
      LoadConfig();

      //    This PlugIn needs a toolbar icon, so request its insertion
	if(m_bvdrPlusShowIcon)
      m_leftclick_tool_id  = InsertPlugInTool(_T(""), _img_vdrPlus, _img_vdrPlus, wxITEM_CHECK,
            _("vdrPlus"), _T(""), NULL,
             vdrPlus_TOOL_POSITION, 0, this);

      m_pDialog = NULL;

      return (
              WANTS_TOOLBAR_CALLBACK    |
              INSTALLS_TOOLBAR_TOOL     |          
              WANTS_CONFIG           
           );
}

bool vdrPlus_pi::DeInit(void)
{
      //    Record the dialog position
      if (NULL != m_pDialog)
      {
            //Capture dialog position
            wxPoint p = m_pDialog->GetPosition();
            SetvdrPlusDialogX(p.x);
            SetvdrPlusDialogY(p.y);
			if ((m_pDialog->m_Timer != NULL) && (m_pDialog->m_Timer->IsRunning())){ // need to stop the timer or crash on exit
				m_pDialog->m_Timer->Stop();
			}
            m_pDialog->Close();
            delete m_pDialog;
            m_pDialog = NULL;

			m_bShowvdrPlus = false;
			SetToolbarItemState( m_leftclick_tool_id, m_bShowvdrPlus );
      }	
    
    SaveConfig();

    RequestRefresh(m_parent_window); // refresh main window

    return true;
}

int vdrPlus_pi::GetAPIVersionMajor()
{
      return MY_API_VERSION_MAJOR;
}

int vdrPlus_pi::GetAPIVersionMinor()
{
      return MY_API_VERSION_MINOR;
}

int vdrPlus_pi::GetPlugInVersionMajor()
{
      return PLUGIN_VERSION_MAJOR;
}

int vdrPlus_pi::GetPlugInVersionMinor()
{
      return PLUGIN_VERSION_MINOR;
}

wxBitmap *vdrPlus_pi::GetPlugInBitmap()
{
      return _img_vdrPlus_pi;
}

wxString vdrPlus_pi::GetCommonName()
{
      return _("vdrPlus");
}


wxString vdrPlus_pi::GetShortDescription()
{
      return _("vdrPlus player");
}

wxString vdrPlus_pi::GetLongDescription()
{
      return _("For playing/controlling playback vdr files");
}

int vdrPlus_pi::GetToolbarToolCount(void)
{
      return 1;
}

void vdrPlus_pi::SetColorScheme(PI_ColorScheme cs)
{
      if (NULL == m_pDialog)
            return;

      DimeWindow(m_pDialog);
}

void vdrPlus_pi::OnToolbarToolCallback(int id)
{
	bool starting = false;

	double scale_factor = GetOCPNGUIToolScaleFactor_PlugIn();
	if (scale_factor != m_GUIScaleFactor) starting = true;
    
	if(NULL == m_pDialog)
      {
            m_pDialog = new Dlg(m_parent_window);
            m_pDialog->plugin = this;
			m_pDialog->SetScaledBitmap(scale_factor);
			m_pDialog->m_Timer = new wxTimer(m_pDialog);
			//m_pDialog->m_Timer = NULL;
			m_pDialog->m_pSlider->SetValue(0);
            m_pDialog->Move(wxPoint(m_hr_dialog_x, m_hr_dialog_y));			
      }

	  m_pDialog->Fit();
	  //Toggle 
	  m_bShowvdrPlus = !m_bShowvdrPlus;	  

      //    Toggle dialog? 
      if(m_bShowvdrPlus) {
		  if (starting) {
			  m_GUIScaleFactor = scale_factor;
			  m_pDialog->SetScaledBitmap(m_GUIScaleFactor);
		  }
          m_pDialog->Show();         
      } else
          m_pDialog->Hide();
     
      // Toggle is handled by the toolbar but we must keep plugin manager b_toggle updated
      // to actual status to ensure correct status upon toolbar rebuild
      SetToolbarItemState( m_leftclick_tool_id, m_bShowvdrPlus);

      RequestRefresh(m_parent_window); // refresh main window
}

bool vdrPlus_pi::LoadConfig(void)
{
      wxFileConfig *pConf = (wxFileConfig *)m_pconfig;

      if(pConf)
      {
            pConf->SetPath ( _T( "/Settings/vdrPlus_pi" ) );
			pConf->Read ( _T( "ShowvdrPlusIcon" ), &m_bvdrPlusShowIcon, 1 );
           
            m_hr_dialog_x =  pConf->Read ( _T ( "DialogPosX" ), 40L );
            m_hr_dialog_y =  pConf->Read ( _T ( "DialogPosY" ), 140L );
         
            if((m_hr_dialog_x < 0) || (m_hr_dialog_x > m_display_width))
                  m_hr_dialog_x = 40;
            if((m_hr_dialog_y < 0) || (m_hr_dialog_y > m_display_height))
                  m_hr_dialog_y = 140;

            return true;
      }
      else
            return false;
}

bool vdrPlus_pi::SaveConfig(void)
{
      wxFileConfig *pConf = (wxFileConfig *)m_pconfig;

      if(pConf)
      {
            pConf->SetPath ( _T ( "/Settings/vdrPlus_pi" ) );
			pConf->Write ( _T ( "ShowvdrPlusIcon" ), m_bvdrPlusShowIcon );
          
            pConf->Write ( _T ( "DialogPosX" ),   m_hr_dialog_x );
            pConf->Write ( _T ( "DialogPosY" ),   m_hr_dialog_y );
            
            return true;
      }
      else
            return false;
}

void vdrPlus_pi::OnvdrPlusDialogClose()
{
    m_bShowvdrPlus = false;
    SetToolbarItemState( m_leftclick_tool_id, m_bShowvdrPlus);
    m_pDialog->Hide();
   // SaveConfig();

    RequestRefresh(m_parent_window); // refresh main window
}





