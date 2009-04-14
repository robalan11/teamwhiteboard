#include "prompt.h"

Prompt::Prompt(const wxString& title)
       : wxFrame(NULL, -1, title, wxPoint(-1, -1), wxSize(450, 400))
{
	// The panel itself
	wxPanel *panel = new wxPanel(this, -1);

	// The window sizers
	wxBoxSizer *vbox = new wxBoxSizer(wxVERTICAL);
	wxGridSizer *gbox = new wxGridSizer(3, 3, 10, 10);
	wxBoxSizer *hbox = new wxBoxSizer(wxHORIZONTAL);


}