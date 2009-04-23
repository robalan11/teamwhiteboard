#include "statusWindow.h"	// ISA status window

StatusWindow::StatusWindow(const wxString& title)
       : wxFrame(NULL, -1, title, wxPoint(50, 50), wxSize(300, 300), 
	   wxMINIMIZE_BOX | wxMAXIMIZE_BOX | wxRESIZE_BORDER | wxSYSTEM_MENU | wxCAPTION | wxCLIP_CHILDREN)
{

	// Add the text box
	mpTextBox = new wxTextCtrl(this, wxID_ANY, wxT(""), 
		wxPoint(-1, -1), wxSize(300, 300), wxTE_MULTILINE | wxTE_READONLY);

}

// The append function
void StatusWindow::Append(const wxString& text)
{
	mpTextBox->AppendText(text);
}