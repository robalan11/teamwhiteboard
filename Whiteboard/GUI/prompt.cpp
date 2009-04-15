#include "prompt.h"
#include "textWindow.h"

// Defines
#define wxID_ANSWER1 3000
#define wxID_ANSWER2 3001
#define wxID_ANSWER3 3002

// Variables that need to be viewed elsewhere
wxFrame *parentWindow;
wxTextCtrl *name_answer;
wxComboBox *sc_answer;
wxTextCtrl *ip_answer;

// The constructor
Prompt::Prompt(const wxString& title, wxFrame *parent)
       : wxFrame(NULL, -1, title, wxPoint(-1, -1), wxSize(290, 200), wxMINIMIZE_BOX | wxCAPTION | wxSYSTEM_MENU)
{
	// Give the frame an icon
	SetIcon(wxICON(mondrian));

	// Disable the chat window
	parentWindow = parent;
	parentWindow->Disable();

	// The panel itself
	wxPanel *panel = new wxPanel(this, -1);

	// The window sizers
	wxBoxSizer *vbox = new wxBoxSizer(wxVERTICAL);
	wxGridSizer *gbox = new wxGridSizer(3, 2, 15, 0);
	wxBoxSizer *hbox = new wxBoxSizer(wxHORIZONTAL);

	// First, add the question/answer fields
	wxStaticText *name_question = new wxStaticText(panel, wxID_ANY, "Name");
	gbox->Add(name_question, wxEXPAND);
	name_answer = new wxTextCtrl(panel, wxID_ANSWER1, _T(""));
	gbox->Add(name_answer, wxEXPAND);
	wxStaticText *sc_question = new wxStaticText(panel, wxID_ANY, "Server or client?");
	gbox->Add(sc_question, wxEXPAND);
	wxString choices[] = {_T("Client"), _T("Server")};
	sc_answer = new wxComboBox(panel, wxID_ANSWER2, "Client", wxDefaultPosition, 
										   wxDefaultSize, 2, choices, wxCB_DROPDOWN | wxCB_READONLY);
	gbox->Add(sc_answer, wxALIGN_LEFT);
	wxStaticText *ip_question = new wxStaticText(panel, wxID_ANY, "IP Address");
	gbox->Add(ip_question, wxEXPAND);
	ip_answer = new wxTextCtrl(panel, wxID_ANSWER3, _T(""));
	gbox->Add(ip_answer, wxEXPAND);

	// Next, add the buttons
	wxButton *ok = new wxButton(panel, wxID_OK, wxT("OK"));
	hbox->Add(ok, 0);
	wxButton *cancel = new wxButton(panel, wxID_CANCEL, wxT("Cancel"));
	hbox->Add(cancel, 0, wxLEFT | wxBOTTOM , 5);

	// Put the window together
	vbox->Add(gbox, 0, wxLEFT | wxTOP, 10);
	vbox->Add(1, 25);
	vbox->Add(hbox, 0, wxALIGN_RIGHT | wxRIGHT, 10);
	panel->SetSizer(vbox);
	Centre();
}

// The OK event handler
void Prompt::OnOK(wxCommandEvent& WXUNUSED(event))
{
	wxString thisName, thisIP;

	// Check for name
	if (name_answer->GetValue() == _T("")){
		wxMessageDialog *dial = new wxMessageDialog(NULL, 
			wxT("Enter a name"), wxT("Exclamation"), 
			wxOK | wxICON_EXCLAMATION);
			dial->ShowModal();
		return;
	}
	thisName = name_answer->GetValue();

	// Check for IP
	if (sc_answer->GetValue() == _T("Client")){
		if (ip_answer->GetValue() == _T("")){
			wxMessageDialog *dial = new wxMessageDialog(NULL, 
				wxT("Enter an IP address"), wxT("Exclamation"), 
				wxOK | wxICON_EXCLAMATION);
				dial->ShowModal();
			return;
		}
		thisIP = ip_answer->GetValue();
	}else{
		thisIP = _T("none");
	}
	
	// Initialize the chat window
	((textWindow *)parentWindow)->initialize(thisName, thisIP);
	parentWindow->Enable();
	Close(true);
}

// The cancel event handler
void Prompt::OnCancel(wxCommandEvent& WXUNUSED(event))
{
	((textWindow *)parentWindow)->Quit();
	Close(true);
}

// The combo box event handler
void Prompt::ChangeIPField(wxCommandEvent& WXUNUSED(event))
{
	if (sc_answer->GetValue() == _T("Server")){
		ip_answer->Disable();
	}else{
		ip_answer->Enable();
	}
}

// The event table
BEGIN_EVENT_TABLE(Prompt, wxFrame)
  EVT_BUTTON(wxID_OK,  Prompt::OnOK)
  EVT_BUTTON(wxID_CANCEL, Prompt::OnCancel)
  EVT_COMBOBOX(wxID_ANSWER2, Prompt::ChangeIPField)
END_EVENT_TABLE()