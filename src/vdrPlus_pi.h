/******************************************************************************
 *
 * Project:  OpenCPN
 * Purpose:  vdrPlus Plugin
 * Author:   Mike Rossiter
 *
 ***************************************************************************
 *   Copyright (C) 2016 by Mike Rossiter                                   *
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

#ifndef _VDR_PI_H_
#define _VDR_PI_H_

#include "wx/wxprec.h"

#ifndef  WX_PRECOMP
  #include "wx/wx.h"
  #include <wx/glcanvas.h>
#endif //precompiled headers

#include <wx/fileconf.h>


#include "ocpn_plugin.h" //Required for OCPN plugin functions
#include "vdrPlusgui_impl.h"
#include <wx/bitmap.h>

#define     PLUGIN_VERSION_MAJOR    0
#define     PLUGIN_VERSION_MINOR    1

#define     MY_API_VERSION_MAJOR    1
#define     MY_API_VERSION_MINOR    6

class Dlg;




//----------------------------------------------------------------------------------------------------------
//    The PlugIn Class Definition
//----------------------------------------------------------------------------------------------------------

#define vdrPlus_TOOL_POSITION    -1          // Request default positioning of toolbar tool

class vdrPlus_pi : public opencpn_plugin_16
{
public:
      vdrPlus_pi(void *ppimgr);
	   ~vdrPlus_pi(void);

//    The required PlugIn Methods
      int Init(void);
      bool DeInit(void);

      int GetAPIVersionMajor();
      int GetAPIVersionMinor();
      int GetPlugInVersionMajor();
      int GetPlugInVersionMinor();
      wxBitmap *GetPlugInBitmap();
      wxString GetCommonName();
      wxString GetShortDescription();
      wxString GetLongDescription();

//    The required override PlugIn Methods
      int GetToolbarToolCount(void);
      void OnToolbarToolCallback(int id);
     

//    Optional plugin overrides
      void SetColorScheme(PI_ColorScheme cs);


//    The override PlugIn Methods

//    Other public methods
      void SetvdrPlusDialogX         (int x){ m_hr_dialog_x = x;};
      void SetvdrPlusDialogY         (int x){ m_hr_dialog_y = x;};
      void SetvdrPlusDialogWidth     (int x){ m_hr_dialog_width = x;};
      void SetvdrPlusDialogHeight    (int x){ m_hr_dialog_height = x;};      
	  void OnvdrPlusDialogClose();

	  int  m_hr_dialog_x, m_hr_dialog_y;	

	  
	  
private:
	double           m_GUIScaleFactor;
	  void OnClose( wxCloseEvent& event );
	  
	  vdrPlus_pi *plugin;
	  
      Dlg         *m_pDialog;

	  wxFileConfig      *m_pconfig;
      wxWindow          *m_parent_window;
      bool              LoadConfig(void);
      bool              SaveConfig(void);
	  
      int				m_hr_dialog_width,m_hr_dialog_height;
      int               m_display_width, m_display_height;      
      int				m_leftclick_tool_id;
	  bool				m_bvdrPlusShowIcon;
	  bool				m_bShowvdrPlus;
};



#endif
