#include "admin.h"

wxListCtrl *people_list = new wxListCtrl();

AdminWindow::AdminWindow(const wxString& title)
       : wxFrame(NULL, -1, title, wxPoint(-1, -1), wxSize(300, 300))
{
public:
	// Add the people box
	people_list->Create(this, -1, wxDefaultPosition, wxDefaultSize, wxLC_LIST|wxLC_SORT_ASCENDING|wxLC_SINGLE_SEL|wxLC_HRULES);
	
}

// The append function
void AdminWindow::OnKickEvent(wxSocketEvent& event)
{
	;
}

BEGIN_EVENT_TABLE(AdminWindow, wxFrame)
END_EVENT_TABLE()