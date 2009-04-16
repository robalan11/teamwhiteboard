#include "statusWindow.h"

wxTextCtrl *textBox;

StatusWindow::StatusWindow(const wxString& title)
       : wxFrame(NULL, -1, title, wxPoint(-1, -1), wxSize(300, 300))
{

	// Add the text box
	textBox = new wxTextCtrl(this, wxID_ANY, wxT(""), 
		wxPoint(-1, -1), wxSize(300, 300), wxTE_MULTILINE | wxTE_READONLY);

}

// The append function
void StatusWindow::Append(const wxString& text)
{
	textBox->AppendText(text);
}