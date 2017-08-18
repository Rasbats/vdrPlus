///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Dec 21 2016)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "vdrPlusgui.h"

///////////////////////////////////////////////////////////////////////////

vdrPlusBase::vdrPlusBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints(wxDefaultSize, wxDefaultSize);

	wxFlexGridSizer* fgSizer2;
	fgSizer2 = new wxFlexGridSizer(3, 0, 0, 0);
	fgSizer2->SetFlexibleDirection(wxBOTH);
	fgSizer2->SetNonFlexibleGrowMode(wxFLEX_GROWMODE_SPECIFIED);

	wxFlexGridSizer* fgSizer3;
	fgSizer3 = new wxFlexGridSizer(0, 15, 0, 0);
	fgSizer3->SetFlexibleDirection(wxBOTH);
	fgSizer3->SetNonFlexibleGrowMode(wxFLEX_GROWMODE_SPECIFIED);

	wxArrayString m_cFileSetTimeChoices;
	m_cFileSetTime = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_cFileSetTimeChoices, 0);
	m_cFileSetTime->SetSelection(0);
	fgSizer3->Add(m_cFileSetTime, 0, wxALL, 5);

	m_bpOpenFile = new wxBitmapButton(this, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW);
	fgSizer3->Add(m_bpOpenFile, 0, wxALL, 5);

	m_bpAnalyse = new wxBitmapButton(this, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW);
	fgSizer3->Add(m_bpAnalyse, 0, wxALL, 5);

	m_bpAnalyse->SetBitmapHover(wxNullBitmap);
	m_bpAnalyse->SetToolTip(wxT("Analyse has to be used before playing"));

	wxString m_cMultiplierChoices[] = { wxT("0"), wxT("1"), wxT("2"), wxT("3"), wxT("4"), wxT("5") };
	int m_cMuliplierNChoices = sizeof(m_cMultiplierChoices) / sizeof(wxString);
	m_cMultiplier = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_cMuliplierNChoices, m_cMultiplierChoices, 0);
	m_cMultiplier->SetSelection(0);
	m_cMultiplier->SetToolTip(wxT("Playback Speed multiplier"));

	fgSizer3->Add(m_cMultiplier, 0, wxALL | wxEXPAND, 5);

	fgSizer3->Add(0, 0, 1, wxEXPAND, 5);

	m_pSlider = new wxSlider(this, wxID_ANY, speed, 1, 100, wxDefaultPosition, wxSize(90, -1), wxSL_HORIZONTAL);
	m_pSlider->SetToolTip(wxT("Playback Speed control"));

	fgSizer3->Add(m_pSlider, 0, wxALL, 5);

	fgSizer3->Add(0, 0, 1, wxEXPAND, 5);

	m_bpPlay = new wxBitmapButton(this, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW);
	fgSizer3->Add(m_bpPlay, 0, wxALL, 5);

	m_bpPause = new wxBitmapButton(this, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW);
	fgSizer3->Add(m_bpPause, 0, wxALL, 5);

	m_bpStop = new wxBitmapButton(this, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW);
	fgSizer3->Add(m_bpStop, 0, wxALL, 5);

	m_bpChooseTime = new wxBitmapButton(this, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW);
	fgSizer3->Add(m_bpChooseTime, 0, wxALL, 5);

	m_bpSetTime = new wxBitmapButton(this, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW);
	fgSizer3->Add(m_bpSetTime, 0, wxALL, 5);

	m_bpSettings = new wxBitmapButton(this, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW);
	fgSizer3->Add(m_bpSettings, 0, wxALL, 5);


	fgSizer2->Add(fgSizer3, 1, wxEXPAND, 5);

	m_pgauge = new wxGauge(this, wxID_ANY, 100, wxDefaultPosition, wxDefaultSize, wxGA_HORIZONTAL);
	fgSizer2->Add(m_pgauge, 1, wxALL | wxEXPAND, 2);

	m_textCtrlSetTime = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE | wxTE_WORDWRAP);
	fgSizer2->Add(m_textCtrlSetTime, 0, wxALL | wxEXPAND, 5);

	this->SetSizer(fgSizer2);
	this->Layout();

	this->Centre(wxBOTH);

	if (m_bpOpenFile){
		m_bpOpenFile->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(vdrPlusBase::OnOpenFile), NULL, this);
	}
	
	// Connect Events
	m_pSlider->Connect(wxEVT_COMMAND_SLIDER_UPDATED, wxCommandEventHandler(vdrPlusBase::OnSliderUpdated), NULL, this);

	m_bpAnalyse->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(vdrPlusBase::OnAnalyse), NULL, this);
	m_bpPlay->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(vdrPlusBase::OnStart), NULL, this);
	m_bpPause->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(vdrPlusBase::OnPause), NULL, this);
	m_bpStop->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(vdrPlusBase::OnStop), NULL, this);

	m_bpChooseTime->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(vdrPlusBase::OnCalendarShow), NULL, this);
	m_bpSetTime->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(vdrPlusBase::OnSetTime), NULL, this);
	
	m_cMultiplier->Connect(wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler(vdrPlusBase::OnChoiceSpeed), NULL, this);

	
	this->Connect(wxEVT_TIMER, wxTimerEventHandler(vdrPlusBase::OnTimer), NULL, this);
	this->Connect(wxEVT_CLOSE_WINDOW, wxCloseEventHandler(vdrPlusBase::OnClose));

}

vdrPlusBase::~vdrPlusBase()
{

	// Disconnect Events
	m_pSlider->Disconnect(wxEVT_COMMAND_SLIDER_UPDATED, wxCommandEventHandler(vdrPlusBase::OnSliderUpdated), NULL, this);

	m_bpAnalyse->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(vdrPlusBase::OnAnalyse), NULL, this);
	m_bpPlay->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(vdrPlusBase::OnStart), NULL, this);
	m_bpPause->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(vdrPlusBase::OnPause), NULL, this);
	m_bpStop->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(vdrPlusBase::OnStop), NULL, this);

	m_bpChooseTime->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(vdrPlusBase::OnCalendarShow), NULL, this);
	m_bpSetTime->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(vdrPlusBase::OnSetTime), NULL, this);

	this->Disconnect(wxEVT_TIMER, wxTimerEventHandler(vdrPlusBase::OnTimer), NULL, this);
	this->Disconnect(wxEVT_CLOSE_WINDOW, wxCloseEventHandler(vdrPlusBase::OnClose));
}

CursorDataBase::CursorDataBase( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : wxPanel( parent, id, pos, size, style )
{
	wxFlexGridSizer* fgSizer4;
	fgSizer4 = new wxFlexGridSizer( 0, 1, 0, 0 );
	fgSizer4->SetFlexibleDirection( wxBOTH );
	fgSizer4->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	progress = new wxGauge( this, wxID_ANY, 100, wxDefaultPosition, wxDefaultSize, wxGA_HORIZONTAL );
	progress->SetValue( 0 ); 
	fgSizer4->Add( progress, 100, wxALL, 5 );
	
	
	this->SetSizer( fgSizer4 );
	this->Layout();
}

CursorDataBase::~CursorDataBase()
{
}
