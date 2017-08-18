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


#include "vdrPlusgui_impl.h"
#include <wx/progdlg.h>
#include <wx/wx.h>
#include "wx/dir.h"
#include <list>
#include "vdrPlus_pi.h"
#include <wx/filesys.h>
#include <wx/wfstream.h>
#include <wx/url.h>
#include <wx/filename.h>
#include <wx/filefn.h>
#include <wx/bitmap.h>

#include "folder.xpm"

#include <windows.h>
#include <stdio.h>
#include <aclapi.h> 

#include <wx/timer.h>


#define BUFSIZE 0x10000

class CalendarDialog;

Dlg::Dlg(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style) : vdrPlusBase(parent, id, title, pos, size, style)
{
	this->Fit();
	dbg = false; //for debug output set to true
}

void Dlg::OnTimer(wxTimerEvent& event){
  Notify();
}


void Dlg::OnOpenFile(wxCommandEvent& event)
{
	
	m_cFileSetTime->Clear();
	
	m_bAnalysed = false;

	wxFileDialog *dialog = new wxFileDialog(NULL, _("Select a VDR file"), m_vdr_dir,
		_T(""), wxT("VDR files (*.txt)|*.txt|All files (*)|*.*"), wxFD_OPEN | wxFD_FILE_MUST_EXIST
		, wxDefaultPosition, wxDefaultSize, _T("File Dialog"));

	if (dialog->ShowModal() == wxID_OK) {

		m_vdr_dir = dialog->GetDirectory();
		m_ifilename = dialog->GetPath();

		m_cFileSetTime->Append(m_ifilename);
		m_cFileSetTime->SetSelection(0);

		wxString str = _T("*** Please analyse before playing the recording ***");

		m_textCtrlSetTime->SetValue(str);

	}
}

void Dlg::OnAnalyse(wxCommandEvent& event) {

	if (m_ifilename == wxEmptyString){
		wxMessageBox(_T("Please select a file first"));
		return;
	}
	
	isLargeFile = false;
	
	m_istream.Open(m_ifilename);
	range = m_istream.GetLineCount();
	m_istream.Close();

	m_pgauge->SetRange(range);

	wxFileInputStream input(m_ifilename);  // try to improve speed with large VDR files
	wxTextInputStream text(input);

	int sz = input.GetSize();
	sz /= 1000;
	//wxMessageBox(wxString::Format(_T("%i"), (int)sz));
	wxString str = _T("NMEA Sentence Identifiers will appear here! Please wait.");
	
	if (sz > 2000){
		isLargeFile = true;
		m_textCtrlSetTime->SetValue(str);
		wxMessageBox(_T("Large files can take a long time to process"));	
	}
	
	wxString nmeaDT;
	wxString id;
	wxString idstring;
	bool found = false;
	bool foundDateTime = true;

	wxDateTime dts, dte, dtTest;

	id = Identify(); // find the date...date/time id

	if (id == wxEmptyString){
		wxMessageBox(_T("No date/time identifiers \nSome features will not be available"));
		foundDateTime = false;
	}

	m_sTimeID = id;   // for use in notify

	idstring = GetIdentifiers(id);

	//wxMessageBox(_T("here"));

	m_textCtrlSetTime->SetValue(idstring);

	if (!foundDateTime || idstring == wxEmptyString){ 
		m_textCtrlSetTime->SetValue(_T("AIS Sentences only"));
		m_bAnalysed = true;
		return;
	}

	while (input.IsOk() && !input.Eof()){
		
		if (!found){
			str = text.ReadLine();
			nmeaDT = ParseNMEASentence(str, id);

			if (nmeaDT != wxEmptyString){

				if (nmeaDT.Length() == 8){
					m_sStartDT = GetDateAndTime(nmeaDT);
				}
				else
					m_sStartDT = nmeaDT;
				dts.ParseDateTime(m_sStartDT);
				dtTest = dts;
				found = true;
			}
		}
		if (found){
			str = text.ReadLine();
			nmeaDT = ParseNMEASentence(str, id);

			if (nmeaDT != wxEmptyString){

				if (nmeaDT.Length() == 8){
					m_sEndDT = GetDateAndTime(nmeaDT);
				}
				else
					m_sEndDT = nmeaDT;

				dte.ParseDateTime(m_sEndDT);
				if (dte > dtTest){
					dtTest = dte;
				}
			}
		}
	}

	m_sEndDT = dte.Format(_T("%Y-%m-%d %H:%M:%S"));

	wxString duration = GetDuration(m_sStartDT, m_sEndDT);

	m_cFileSetTime->Append(m_sStartDT);
	m_cFileSetTime->Append(m_sEndDT);
	m_cFileSetTime->Append(duration);

	input.~wxFileInputStream();
	text.~wxTextInputStream();

	m_bAnalysed = true;

}

wxString Dlg::GetIdentifiers(wxString id){

	wxFileInputStream input(m_ifilename);
	wxTextInputStream text(input);

	wxString str;
	wxString token[40];
	wxString idout = wxEmptyString;
	int countID = 0;
	bool cont = false;

	while (input.IsOk() && !input.Eof()){

		str = text.ReadLine();

		token[0] = _T("");

		wxStringTokenizer tokenizer(str, wxT(","));
		int i = 0;

		while (tokenizer.HasMoreTokens())	{
			token[i] = tokenizer.GetNextToken();
			i++;
		}

		if ((token[0].Right(3) == id)){
			cont = true;
			countID++;
			if (countID == 2){
				input.~wxFileInputStream();
				text.~wxTextInputStream();
				return  idout.BeforeLast(',', NULL);
			}
		}

		if (cont){
			idout += token[0].Right(3) + _T(", ");
		}
	}
	return wxEmptyString;
}
wxString Dlg::Identify(){

	wxFileInputStream input(m_ifilename);
	wxTextInputStream text(input);

	int counter = 0;
	wxString str;
	wxString nmeaIdent;

	bool fGLL = false;
	bool fGGA = false;
	bool fRMC = false;

	while (input.IsOk() && !input.Eof()){

		str = text.ReadLine();

		nmeaIdent = ParseNMEAIdentifier(str);

		if (nmeaIdent == _T("RMC"))
		{
			input.~wxFileInputStream();
			text.~wxTextInputStream();
			return _T("RMC");
			// statements to be executed if testExpression1 and testExpression2 is false and testExpression3 is true
		}
		else if (nmeaIdent == _T("ZDA"))
		{
			//m_istream.Close();
			input.~wxFileInputStream();
			text.~wxTextInputStream();
			return _T("ZDA");  //first priority for date/time sentence
			// statements to be executed if testExpression1 and testExpression2 is false and testExpression3 is true
		}
		else if (nmeaIdent == _T("GLL"))
		{
			counter++;
			fGLL = true;
			if (counter == 3) break;
			// statements to be executed if testExpression1 is true
		}
		else if (nmeaIdent == _T("GGA"))
		{
			counter++;
			fGGA = true;
			if (counter == 3) break;
			// statements to be executed if testExpression1 is false and testExpression2 is true
		}

		else
		{

			// statements to be executed if all test expressions are false
		}
	}

	if (fGLL)
	{
		input.~wxFileInputStream();
		text.~wxTextInputStream();
		return _T("GLL");
		// statements to be executed if testExpression1 is false and testExpression2 is true
	}
	else if (fGGA)
	{
		input.~wxFileInputStream();
		text.~wxTextInputStream();
		return _T("GGA");
		// statements to be executed if testExpression1 and testExpression2 is false and testExpression3 is true
	}

	else return wxEmptyString;
}

wxString Dlg::ParseNMEAIdentifier(wxString sentence)
{
	wxString token[40];
	token[0] = _T("");

	wxStringTokenizer tokenizer(sentence, wxT(","));
	int i = 0;

	while (tokenizer.HasMoreTokens())	{
		token[i] = tokenizer.GetNextToken();
		i++;
	}

	return token[0].Right(3);
}

wxString Dlg::ParseNMEASentence(wxString sentence, wxString id)
{

	wxString token[40];
	wxString s0, s1, s2, s3, s4, s5, s6, s7, s8, s9, s10, s11;
	token[0] = _T("");

	wxStringTokenizer tokenizer(sentence, wxT(","));
	int i = 0;

	while (tokenizer.HasMoreTokens())	{
		token[i] = tokenizer.GetNextToken();
		i++;
	}

	//wxMessageBox(token[0].Right(3));

	if ((token[0].Right(3) == _T("GGA")) && (token[0].Right(3) == id)){
		//$--GGA,hhmmss.ss,llll.ll,a,yyyyy.yy,a,x,xx,x.x,x.x,M,x.x,M,x.x,xxxx*hh<CR><LF>
		s0 = token[1];
		s1 = s0.Mid(0, 2);
		s2 = s0.Mid(2, 2);
		s3 = s0.Mid(4, 2);
		s4 = s1 + _T(":") + s2 + _T(":") + s3;
		m_sStartTime = s4;
		//m_bGotStartTime = true;
		return _T("1999-01-01 ") + m_sStartTime;
	}
	else if ((token[0].Right(3) == _T("GLL")) && (token[0].Right(3) == id)){
		//$IIGLL,5027.776667,N,412.690754,W,123327,A*26

		s0 = token[5];
		s1 = s0.Mid(0, 2);
		s2 = s0.Mid(2, 2);
		s3 = s0.Mid(4, 2);
		s4 = s1 + _T(":") + s2 + _T(":") + s3;
		m_sStartTime = s4;
		//m_bGotStartTime = true;
		//wxMessageBox(m_sStartTime);

		return _T("1999-01-01 ") + m_sStartTime;
	}
	else if ((token[0].Right(3) == _T("ZDA")) && (token[0].Right(3) == id)){
		//$IIZDA,055842,15,09,2012,,*5D
		s0 = token[1];
		s1 = s0.Mid(0, 2);
		s2 = s0.Mid(2, 2);
		s3 = s0.Mid(4, 2);
		s4 = s1 + _T(":") + s2 + _T(":") + s3;
		s5 = token[2];
		s6 = token[3];
		s7 = token[4];
		s9 = s7 + _T("-") + s6 + _T("-") + s5 + _T(" ") + s4;

		m_sStartDateTime = s9;// _T("$IIZDA,055842,15,09,2012,,*5D");  // 
		return m_sStartDateTime;
	}
	else if ((token[0].Right(3) == _T("RMC")) && (token[0].Right(3) == id)){
		//$GPRMC, 110858.989, A, 4549.9135, N, 00612.2671, E, 003.7, 207.5, 050513, , , A * 60
		//$GPRMC,110858.989,A,4549.9135,N,00612.2671,E,003.7,207.5,050513,,,A*60

		s10 = token[3] + token[4];
		s11 = token[5] + token[6];


		s0 = token[1];
		s1 = s0.Mid(0, 2);
		s2 = s0.Mid(2, 2);
		s3 = s0.Mid(4, 2);
		s4 = s1 + _T(":") + s2 + _T(":") + s3;
		s5 = token[9];
		s6 = s5.Mid(0, 2);
		s7 = s5.Mid(2, 2);
		s8 = _T("20") + s5.Mid(4, 2);

		s9 = s8 + _T("-") + s7 + _T("-") + s6 + _T(" ") + s4;
		m_sStartDateTime = s9;

		return m_sStartDateTime;
	}

	else {

		return wxEmptyString;
	}

}

void Dlg::OnStart(wxCommandEvent& event) {
	if (m_ifilename == wxEmptyString){
		wxMessageBox(_T("Please select and analyse a file first"));
		return;
	}
	if (!m_bAnalysed){
		wxMessageBox(_T("Please analyse the file first"));
		return;
	}

	if (isLargeFile){
		wxString msg = _T("Large file ... this may take some time. Please wait!");
		wxString msg2 = _T("This may take some time. Please wait!");
		m_textCtrlSetTime->SetValue(msg);
		wxMessageBox(msg2);		
	}

	m_iAdvanceMultiple = m_cMultiplier->GetSelection();
	m_istream.Open(m_ifilename);	

	if (m_bUseSetTime){
		m_istream.GoToLine(m_iPos);
		this->SetProgress(m_iPos);
	}
	else if (m_bUseStop) {		
		m_istream.GoToLine(1);		
	}
	else if (m_bUsePause){
		this->SetProgress(m_iPos);
	}
	m_bUsePause = false;
	m_bUseStop = false;
	m_bUseSetTime = false;

	m_Timer->Start(m_interval, wxTIMER_CONTINUOUS); // start timer

}

void Dlg::OnPause(wxCommandEvent& event) {
	
	if (m_Timer->IsRunning()) m_Timer->Stop();	
	m_interval = m_Timer->GetInterval();
	m_iPos = i;
	if (m_sNmeaTime != wxEmptyString)	wxMessageBox(m_sNmeaTime, _T("Time at Pause"));
	m_bUsePause = true;
}

void Dlg::OnStop(wxCommandEvent& event) {
	
	if (m_Timer->IsRunning()) m_Timer->Stop();

	m_pgauge->SetValue(0);
	wxString msg = _T("Playback has been stopped");
	m_textCtrlSetTime->SetValue(msg);

	m_interval = m_Timer->GetInterval();
	m_bUseSetTime = false;
	m_bUseStop = true;
	//wxMessageBox(wxString::Format(_T("%i"), (int)m_iPos));
	//wxMessageBox(wxString::Format(_T("%i"), (int)range));
}


void Dlg::OnStop() {
	m_pgauge->SetValue(0);
	if (m_Timer->IsRunning()) m_Timer->Stop();
	m_interval = m_Timer->GetInterval();
	m_bUseSetTime = false;
	m_bUseStop = true;
	//wxMessageBox(wxString::Format(_T("%i"), (int)m_iPos));
	//wxMessageBox(wxString::Format(_T("%i"), (int)range));
}


void Dlg::OnChoiceSpeed(wxCommandEvent& event) {
	if (m_Timer->IsRunning()) m_Timer->Stop();
	m_interval = m_Timer->GetInterval();
	m_bUseSetTime = false;
	m_bUseStop = true;

	wxString msg = _T("Playback has been stopped! Please restart.");
	m_textCtrlSetTime->SetValue(msg);
}

void Dlg::OnCalendarShow(wxCommandEvent& event)
{
	//if (m_Timer->IsRunning()) m_Timer->Stop();
	OnStop();

	CalendarDialog CalDialog(this, -1, _("Set Date/Time"),
		wxPoint(100, 100), wxSize(400, 500));

	CalDialog._timeCtrl->SetTime(12, 00, 00);

	wxDateTime sdt;
	sdt.ParseDateTime(m_sStartDT);

	//wxMessageBox(wxString::Format(_T("%Y-%m-%d %H:%M:%S"), sdt));

	CalDialog.dialogCalendar->SetDate(sdt);

	int hh = sdt.GetHour();
	int mm = sdt.GetMinute();
	int ss = sdt.GetSecond();

	//wxMessageBox(wxString::Format(_T("%i"), hh));
	//wxMessageBox(wxString::Format(_T("%i"), mm));
	//wxMessageBox(wxString::Format(_T("%i"), ss));

	CalDialog._timeCtrl->SetTime(hh, mm, ss);

	if (CalDialog.ShowModal() == wxID_OK){

		wxDateTime dt;
		wxDateTime dm = CalDialog.dialogCalendar->GetDate();

		int hh, mm, ss;
		CalDialog._timeCtrl->GetTime(&hh, &mm, &ss);

		wxTimeSpan mts = wxTimeSpan(hh, mm);


		dt = dm.Add(mts);

		m_textCtrlSetTime->SetValue(dt.Format(_T("%Y-%m-%d %H:%M:%S")));

		m_sSetTime = dt.Format(_T("%Y-%m-%d %H:%M:%S"));
		//m_bUseSetTime = true;
	}
}

void Dlg::OnSetTime(wxCommandEvent& event) {

	if (m_ifilename == wxEmptyString){
		wxMessageBox(_T("Please select and analyse a file first"));
		return;
	}
	if (!m_bAnalysed){
		wxMessageBox(_T("Please analyse the file first"));
		return;
	}

	wxString str;
	wxString nmeaDT;
	wxString settime;

	m_bUseSetTime = false;

	settime = m_textCtrlSetTime->GetValue();
	wxDateTime dt;
	bool date = dt.ParseDateTime(settime);
	if (!date){
		wxMessageBox(_T("Please use the calendar to select a date/time"));
		return;
	}

	
	
	wxString msg = _T("Advancing playback to the selected time. Please wait!");
	if (isLargeFile){
		msg += _T("\nThis is a large file!");
	}
	
	wxMessageBox(msg);

	wxString start = m_cFileSetTime->GetString(1);	
	wxString end = m_cFileSetTime->GetString(2);	

	if (!GetCalendarInRange(start, end, dt)){
		return;
	}

	if (m_Timer->IsRunning()) m_Timer->Stop();
	m_istream.Open(m_ifilename);

	while (nmeaDT != settime){
		int pos = m_istream.GetCurrentLine();

		if (m_istream.Eof() || pos == -1){
			str = m_istream.GetFirstLine();
			pos = m_istream.GetCurrentLine();
		}
		else
			str = m_istream.GetNextLine();

		nmeaDT = ParseNMEASentence(str, m_sTimeID);

		if (nmeaDT != wxEmptyString && nmeaDT != settime){
			m_sNmeaTime = nmeaDT;
		}
		else if (nmeaDT == settime){
			//range = m_istream.GetLineCount();
			//m_istream.Close();
			m_iPos = pos;
			if (m_iPos > range){
				m_iPos -= range;
			}
			m_pgauge->SetValue(m_iPos);
			m_bUseSetTime = true;
			//wxMessageBox(wxString::Format(_T("%i"), m_iPos));
			return;
		}

		//m_iPos = pos;
		PushNMEABuffer(str + _T("\r\n"));
		

	}
}

void Dlg::OnClose(wxCloseEvent& event)
{	
	//if (m_Timer->IsRunning()) m_Timer->Stop();
	plugin->OnvdrPlusDialogClose();
}

void Dlg::Notify()
{
	wxString str;
	wxString nmeaDT = wxEmptyString;
	int diffpos;

	int pos = m_istream.GetCurrentLine();
	i = pos;

	if (m_istream.Eof() || pos == -1){
		str = m_istream.GetFirstLine();
		//pos = m_istream.GetCurrentLine();
	}
	else{

		str = m_istream.GetNextLine();
	}

	if (m_iAdvanceMultiple != 0){


		pos += m_iLinesToAdvance * m_iAdvanceMultiple;

		if (pos > range){
			this->SetProgress(0);
			str = m_istream.GetFirstLine();
			pos = m_istream.GetCurrentLine();
		}
		else

			if (!m_istream.Eof()){
				m_istream.GoToLine(pos);
				str = m_istream.GetNextLine();
			}
			else {
				str = m_istream.GetFirstLine();
				pos = m_istream.GetCurrentLine();
			}
	}
	else {
		if (countTimeID == 2){
			countTimeID = 0;
		}
		//str = m_istream.GetNextLine();
	}

	
	nmeaDT = ParseNMEASentence(str, m_sTimeID);
	if (nmeaDT != wxEmptyString){
		m_sNmeaTime = nmeaDT;
		m_iPosTimeID = i;
		countTimeID++;

		if (countTimeID == 1){
			firstPos = i;
		}
		else if (countTimeID == 2) {
			secondPos = i;
			diffpos = secondPos - firstPos;
			m_iLinesToAdvance = diffpos;
		}
	}

	//m_iPos = pos;

	PushNMEABuffer(str + _T("\r\n"));

	this->SetProgress(pos);
	m_textCtrlSetTime->SetValue(str);
	
}

wxString Dlg::GetDateAndTime(wxString timestring){

	wxString time = timestring;
	return _T("1999-01-01 ") + time;
}

wxString Dlg::GetOrderedDateTime(wxString start, wxString end){
	wxDateTime st, et;
	wxDateSpan myOneDay = wxDateSpan::Days(1);
	st.ParseDateTime(start);
	et.ParseDateTime(end);

	if (et < st){
		et.Add(myOneDay);
		return et.Format(_T("%Y-%m-%d %H:%M:%S"));
	}
	else
		return end;
}

wxString Dlg::GetDuration(wxString start, wxString end){

	wxDateTime st, et;

	st.ParseDateTime(start);
	et.ParseDateTime(end);

	wxTimeSpan diff;
	diff = et.Subtract(st);

	int minutes = diff.GetMinutes();
	wxString out = wxString::Format(_T("%i"), (int)minutes) + _T(" Minutes");
	return out;
}

bool Dlg::GetCalendarInRange(wxString start, wxString end, wxDateTime caldate){

	wxDateTime st, et;

	st.ParseDateTime(start);
	et.ParseDateTime(end);

	if (caldate < st || caldate > et){
		wxMessageBox(_T("Date selected is out of range"));
		return false;
	}
	return true;
}


void Dlg::SetProgress(int progress)
{
	//wxMessageBox(wxString::Format(_T("%i"), (int)progress));
	m_iPos = progress;
	m_pgauge->SetValue(progress);
}

void Dlg::OnSliderUpdated(wxCommandEvent& event)
{
	this->SetInterval(1000 / m_pSlider->GetValue());
}

void Dlg::SetInterval(int interval)
{
	m_interval = interval;
	if (m_Timer->IsRunning()) // Timer started?
		m_Timer->Start(m_interval, wxTIMER_CONTINUOUS); // restart timer with new interval
}


wxBitmap Dlg::GetScaledBitmap(wxBitmap bitmap, const wxString svgFileName, double scale_factor)
{
	int margin = 4;			//there is a small margin around the bitmap drawn by the wxBitmapButton
	int w = bitmap.GetWidth() - margin;
	int h = bitmap.GetHeight() - margin;
	w *= scale_factor;
	h *= scale_factor;

	
#ifdef VDR_USE_SVG

	wxString shareLocn = *GetpSharedDataLocation() +
		_T("plugins") + wxFileName::GetPathSeparator() +
		_T("vdrPlus_pi") + wxFileName::GetPathSeparator()
		+ _T("data") + wxFileName::GetPathSeparator();
	wxString filename = shareLocn + svgFileName + _T(".svg");
	wxBitmap svgbm = GetBitmapFromSVGFile(filename, w, h);
	if (svgbm.GetWidth() > 0 && svgbm.GetHeight() > 0)
		return svgbm;
	else
#endif
	{
		wxImage a = bitmap.ConvertToImage();
		return wxBitmap(a.Scale(w, h), wxIMAGE_QUALITY_HIGH);
	}
}

void Dlg::SetScaledBitmap(double factor)
{
	//  Round to the nearest "quarter", to avoid rendering artifacts
	m_ScaledFactor = wxRound(factor * 4.0) / 4.0;
	//set buttons bitmap

	m_bpOpenFile->SetBitmapLabel(GetScaledBitmap(wxBitmap(openfile), _T("openfile"), m_ScaledFactor));
	m_bpAnalyse->SetBitmapLabel(GetScaledBitmap(wxBitmap(setting), _T("analyse"), m_ScaledFactor));
	m_bpPlay->SetBitmapLabel(GetScaledBitmap(wxBitmap(play), _T("play"), m_ScaledFactor));
	m_bpPause->SetBitmapLabel(GetScaledBitmap(wxBitmap(pause), _T("pause"), m_ScaledFactor));
	m_bpStop->SetBitmapLabel(GetScaledBitmap(wxBitmap(stop), _T("stop"), m_ScaledFactor));
	m_bpChooseTime->SetBitmapLabel(GetScaledBitmap(wxBitmap(calendar_clock), _T("calendar_clock"), m_ScaledFactor));
	m_bpSetTime->SetBitmapLabel(GetScaledBitmap(wxBitmap(find_time), _T("find_time"), m_ScaledFactor));

}


CalendarDialog::CalendarDialog(wxWindow * parent, wxWindowID id, const wxString & title,
	const wxPoint & position, const wxSize & size, long style)
	: wxDialog(parent, id, title, position, size, style)
{

	wxString dimensions = wxT(""), s;
	wxPoint p;
	wxSize  sz;

	sz.SetWidth(440);
	sz.SetHeight(350);

	p.x = 6; p.y = 2;
	s.Printf(_(" x = %d y = %d\n"), p.x, p.y);
	dimensions.append(s);
	s.Printf(_(" width = %d height = %d\n"), sz.GetWidth(), sz.GetHeight());
	dimensions.append(s);
	dimensions.append(wxT("here"));

	dialogCalendar = new wxCalendarCtrl(this, -1, wxDefaultDateTime, p, sz, wxCAL_SHOW_HOLIDAYS, _("Tide Calendar"));

	m_staticText = new wxStaticText(this, wxID_ANY, _("Time:"), wxPoint(15, 360), wxSize(120, 42));

	wxDateTime dt = dt.Now();
	_timeCtrl = new wxTimePickerCtrl(this, wxID_ANY, dt, wxPoint(210, 360), wxSize(120, 42));

	p.y += sz.GetHeight() + 80;
	wxButton * b = new wxButton(this, wxID_OK, _("OK"), p, wxDefaultSize);
	p.x += 220;
	wxButton * c = new wxButton(this, wxID_CANCEL, _("Cancel"), p, wxDefaultSize);

}