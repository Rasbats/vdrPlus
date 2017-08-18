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

#ifndef _HRGUI_IMPL_H_
#define _HRGUI_IMPL_H_

#ifdef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "vdrPlusgui.h"
#include "vdrPlus_pi.h"

// Added
#include <wx/tokenzr.h>
#include <wx/calctrl.h>
#include <wx/timectrl.h>
#include <wx/txtstrm.h>
#include <wx/wfstream.h>
#include <wx/utils.h>
#include <wx/gdicmn.h>

using namespace std;

class vdrPlus_pi;

class Dlg : public vdrPlusBase
{
public:
        Dlg( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("vdrPlus"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1,-1 ), long style = wxCLOSE_BOX|wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER );
        vdrPlus_pi *plugin; 

		double			m_ScaledFactor;
		double			m_GUIScaleFactor;
        void SetScaledBitmap(double factor);
		wxTimer *   m_Timer;
		void OnTimer(wxTimerEvent& event);

private:
		void Notify();
		void SetInterval(int interval);
		int  m_interval = 1;
		void SetProgress(int progress);
		void OnSliderUpdated(wxCommandEvent& event);

		int m_iLinesToAdvance = 0;
		int m_iAdvanceMultiple = 0;
		int m_iLineCount;
		int countTimeID = 0;
		int i;
		int firstPos;
		int secondPos;

		bool isLargeFile;

		wxString m_sTimeSentence;
		wxString m_sTimeID;
		wxString m_sNmeaTime = wxEmptyString;

		int m_iPos;
		int m_iPosTimeID = 999;

		wxString m_sStartDT;
		wxString m_sEndDT;

		bool b_gotDateTime;
		wxString m_sStartTime;
		wxString m_sStopTime;
		wxString m_sStartDateTime;
		wxString m_sStopDateTime;

		wxBitmap GetScaledBitmap(wxBitmap bitmap, const wxString svgFileName, double scale_factor);
		
        bool dbg;
		bool m_bAnalysed = false;

		wxString    m_ifilename;
		wxString	m_sSetTime;
		bool m_bUseSetTime = false;
		bool m_bUseStop = true;
		bool m_bUsePause = false;

		wxTextFile        m_istream;

		// Added
		void OnOpenFile(wxCommandEvent& event);
		void OnAnalyse(wxCommandEvent& event);
		wxString GetIdentifiers(wxString id);
		wxString Identify();
		wxString ParseNMEAIdentifier(wxString sentence);
		wxString ParseNMEASentence(wxString sentence, wxString id);
		void OnStart(wxCommandEvent& event);
		void OnPause(wxCommandEvent& event);
		void OnStop(wxCommandEvent& event);
		void OnStop();
		void OnChoiceSpeed(wxCommandEvent& event);
		void OnCalendarShow(wxCommandEvent& event);
		void OnSetTime(wxCommandEvent& event);
		void OnClose(wxCloseEvent& event);

		wxString GetDateAndTime(wxString timestring);
		wxString GetOrderedDateTime(wxString start, wxString end);
		wxString GetDuration(wxString start, wxString end);
		bool GetCalendarInRange(wxString start, wxString end, wxDateTime caldate);

		wxString m_vdr_dir;
		int range;
		
};


class CalendarDialog : public wxDialog
{
public:

	CalendarDialog(wxWindow * parent, wxWindowID id, const wxString & title,
		const wxPoint & pos = wxDefaultPosition,
		const wxSize & size = wxDefaultSize,
		long style = wxDEFAULT_DIALOG_STYLE);

	wxCalendarCtrl* dialogCalendar;
	wxStaticText *m_staticText;
	wxTimePickerCtrl *_timeCtrl;
	wxString GetText();

private:
	void OnOk(wxCommandEvent & event);

};

#endif


