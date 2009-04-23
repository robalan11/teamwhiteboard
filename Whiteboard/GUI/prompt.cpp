#include "prompt.h"		// ISA prompt
#include "textWindow.h"	// HASA reference; text window is parent

// Defines
#define wxID_ANSWER1 3000
#define wxID_ANSWER2 3001
#define wxID_ANSWER3 3002

// The constructor
Prompt::Prompt(const wxString& title, wxFrame* parent)
       : wxFrame(parent, -1, title, wxPoint(-1, -1), wxSize(290, 200), wxMINIMIZE_BOX | wxCAPTION | wxSYSTEM_MENU | wxFRAME_FLOAT_ON_PARENT)
{
	// Give the frame an icon
	SetIcon(wxICON(mondrian));

	// Disable the chat window
	mpParentWindow = parent;
	mpParentWindow->Disable();

	// The panel itself
	wxPanel* panel = new wxPanel(this, -1);

	// The window sizers
	wxBoxSizer* vbox = new wxBoxSizer(wxVERTICAL);
	wxGridSizer* gbox = new wxGridSizer(3, 2, 15, 0);
	wxBoxSizer* hbox = new wxBoxSizer(wxHORIZONTAL);

	// First, add the question/answer fields
	wxStaticText* name_question = new wxStaticText(panel, wxID_ANY, "Name");
	gbox->Add(name_question, wxEXPAND);
	mpNameAnswer = new wxTextCtrl(panel, wxID_ANSWER1, _T(""));
	gbox->Add(mpNameAnswer, wxEXPAND);
	wxStaticText* sc_question = new wxStaticText(panel, wxID_ANY, "Server or client?");
	gbox->Add(sc_question, wxEXPAND);
	wxString choices[] = {_T("Client"), _T("Server")};
	mpScAnswer = new wxComboBox(panel, wxID_ANSWER2, "Client", wxDefaultPosition, 
										   wxDefaultSize, 2, choices, wxCB_DROPDOWN | wxCB_READONLY);
	gbox->Add(mpScAnswer, wxALIGN_LEFT);
	wxStaticText* ip_question = new wxStaticText(panel, wxID_ANY, "IP Address");
	gbox->Add(ip_question, wxEXPAND);
	mpIpAnswer = new wxTextCtrl(panel, wxID_ANSWER3, _T(""));
	gbox->Add(mpIpAnswer, wxEXPAND);

	// Next, add the buttons
	wxButton* ok = new wxButton(panel, wxID_OK, wxT("OK"));
	hbox->Add(ok, 0);
	wxButton* cancel = new wxButton(panel, wxID_CANCEL, wxT("Cancel"));
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
	wxString this_name, this_ip;

	// Check for name
	if (mpNameAnswer->GetValue() == _T("")){
		wxMessageDialog *dial = new wxMessageDialog(NULL, 
			wxT("Enter a name"), wxT("Exclamation"), 
			wxOK | wxICON_EXCLAMATION);
			dial->ShowModal();
		return;
	}
	this_name = mpNameAnswer->GetValue();

	// Check for IP
	if (mpScAnswer->GetValue() == _T("Client")){
		if (mpIpAnswer->GetValue() == _T("")){
			wxMessageDialog *dial = new wxMessageDialog(NULL, 
				wxT("Enter an IP address"), wxT("Exclamation"), 
				wxOK | wxICON_EXCLAMATION);
				dial->ShowModal();
			return;
		}
		this_ip = mpIpAnswer->GetValue();
	}else{
		this_ip = _T("none");
	}
	
	// Initialize the chat window
	((TextWindow *)mpParentWindow)->Initialize(this_name, this_ip);
	mpParentWindow->Enable();
	Close(true);
}

// The cancel event handler
void Prompt::OnCancel(wxCommandEvent& WXUNUSED(event))
{
	((TextWindow *)mpParentWindow)->Quit();
	Close(true);
}

// The combo box event handler
void Prompt::ChangeIPField(wxCommandEvent& WXUNUSED(event))
{
	if (mpScAnswer->GetValue() == _T("Server")){
		mpIpAnswer->Disable();
	}else{
		mpIpAnswer->Enable();
	}
}

// The event table
BEGIN_EVENT_TABLE(Prompt, wxFrame)
  EVT_BUTTON(wxID_OK,  Prompt::OnOK)
  EVT_BUTTON(wxID_CANCEL, Prompt::OnCancel)
  EVT_COMBOBOX(wxID_ANSWER2, Prompt::ChangeIPField)
END_EVENT_TABLE()