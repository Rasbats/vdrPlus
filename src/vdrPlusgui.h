///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Dec 21 2016)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __VDRPLUSGUI_H__
#define __VDRPLUSGUI_H__

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/string.h>
#include <wx/choice.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/bmpbuttn.h>
#include <wx/button.h>
#include <wx/slider.h>
#include <wx/sizer.h>
#include <wx/dialog.h>

#include <wx/panel.h>
// Added
#include <wx/filepicker.h>
#include <wx/timer.h>
#include <wx/gauge.h>
#include <wx/textctrl.h>
#include "wx/event.h"
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class vdrPlusBase
///////////////////////////////////////////////////////////////////////////////
class vdrPlusBase : public wxDialog 
{
	private:
	
	protected:
		
		wxChoice* m_cMultiplier;
		wxBitmapButton* m_bpOpenFile;
		wxBitmapButton* m_bpAnalyse;
		
		wxGauge* m_pgauge;
		wxTextCtrl* m_textCtrlSetTime;
		wxBitmapButton* m_bpPlay;
		wxBitmapButton* m_bpPause;
		wxBitmapButton* m_bpStop;
		wxBitmapButton* m_bpChooseTime;
		wxBitmapButton* m_bpSetTime;
		wxBitmapButton* m_bpSettings;
		
		// Virtual event handlers, overide them in your derived class

		virtual void OnOpenFile(wxCommandEvent& event) { event.Skip(); }
		virtual void OnSliderUpdated(wxCommandEvent& event) { event.Skip(); }
		virtual void OnAnalyse(wxCommandEvent& event) { event.Skip(); }
		virtual void OnStart(wxCommandEvent& event) { event.Skip(); }
		virtual void OnPause(wxCommandEvent& event) { event.Skip(); }
		virtual void OnStop(wxCommandEvent& event) { event.Skip(); }
		virtual void OnClose(wxCloseEvent& event){ event.Skip(); }

		virtual void OnCalendarShow(wxCommandEvent& event) { event.Skip(); }
		virtual void OnSetTime(wxCommandEvent& event) { event.Skip(); }
		virtual void OnChoiceSpeed(wxCommandEvent& event) { event.Skip(); }
		virtual void OnTimer(wxTimerEvent& event){ event.Skip(); }

	
	public:
		
		vdrPlusBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 538,67 ), long style = wxDEFAULT_DIALOG_STYLE ); 
		~vdrPlusBase();

		int speed;
		wxSlider* m_pSlider;
		wxChoice* m_cFileSetTime;
};

///////////////////////////////////////////////////////////////////////////////
/// Class CursorDataBase
///////////////////////////////////////////////////////////////////////////////
class CursorDataBase : public wxPanel 
{
	private:
	
	protected:
		wxGauge* progress;
	
	public:
		
		CursorDataBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 366,48 ), long style = wxTAB_TRAVERSAL ); 
		~CursorDataBase();

		

	
};

#endif //__VDRPLUSGUI_H__
