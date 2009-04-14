#include "textWindow.h"

wxTextCtrl *tc2;
wxTextCtrl *tc3;

textWindow::textWindow(const wxString& title)
       : wxFrame(NULL, -1, title, wxPoint(-1, -1), wxSize(300, 400))
{

  wxPanel *panel = new wxPanel(this, -1);

  wxBoxSizer *vbox = new wxBoxSizer(wxVERTICAL);



  wxBoxSizer *hbox2 = new wxBoxSizer(wxHORIZONTAL);
  wxStaticText *st2 = new wxStaticText(panel, wxID_ANY, 
      wxT("WEEE Classes"));

  hbox2->Add(st2, 0);
  vbox->Add(hbox2, 0, wxLEFT | wxTOP, 10);

  vbox->Add(-1, 10);

  wxBoxSizer *hbox3 = new wxBoxSizer(wxHORIZONTAL);
  tc2 = new wxTextCtrl(panel, wxID_ANY, wxT(""), 
      wxPoint(-1, -1), wxSize(-1, -1), wxTE_MULTILINE | wxTE_READONLY);

  hbox3->Add(tc2, 1, wxEXPAND);
  vbox->Add(hbox3, 1, wxLEFT | wxRIGHT | wxEXPAND, 10);

  vbox->Add(-1, 25);

  wxBoxSizer *hbox4 = new wxBoxSizer(wxHORIZONTAL);
  tc3 = new wxTextCtrl(panel, 1, wxT(""), 
      wxPoint(-1, -1), wxSize(-1, -1), wxTE_LEFT);

  hbox4->Add(tc3, 1, wxEXPAND);
  vbox->Add(hbox4, 0, wxLEFT | wxRIGHT | wxEXPAND, 10);

  vbox->Add(-1, 25);

  wxBoxSizer *hbox5 = new wxBoxSizer(wxHORIZONTAL);
  wxButton *btn1 = new wxButton(panel, wxID_SAVE, wxT("Send"));
  hbox5->Add(btn1, 0);
  wxButton *btn2 = new wxButton(panel, wxID_EXIT, wxT("Close"));
  hbox5->Add(btn2, 0, wxLEFT | wxBOTTOM , 5);
  vbox->Add(hbox5, 0, wxALIGN_RIGHT | wxRIGHT, 10);

  panel->SetSizer(vbox);

  Centre();

}

void textWindow::OnQuit(wxCommandEvent& WXUNUSED(event))
{
    Close(true);
}

void textWindow::OnTextEnter(wxCommandEvent& WXUNUSED(event))
{
	wxString input = tc3->GetValue() + "\n";
	if (input != "\n"){
		tc2->AppendText(input);
		tc3->Clear();
	}
}

BEGIN_EVENT_TABLE(textWindow, wxFrame)
  EVT_BUTTON(wxID_EXIT,  textWindow::OnQuit)
  EVT_BUTTON(wxID_SAVE, textWindow::OnTextEnter)
  EVT_TEXT_ENTER(1, textWindow::OnTextEnter)
END_EVENT_TABLE()