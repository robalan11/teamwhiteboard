#include "admin.h"

wxListCtrl *people_list = new wxListCtrl();

AdminWindow::AdminWindow(const wxString& title)
       : wxFrame(NULL, -1, title, wxPoint(-1, -1), wxSize(300, 300))
{
	// Add the people box
	people_list->Create(this, -1, wxDefaultPosition, wxDefaultSize, wxLC_LIST|wxLC_SORT_ASCENDING|wxLC_SINGLE_SEL|wxLC_HRULES);
	
}

void AdminWindow::AddPerson(wxString name)
{
	wxListItem *newItem = new wxListItem();
	newItem->SetText(name);
	people_list->InsertItem(*newItem);
}

void AdminWindow::RemovePerson(wxString name)
{
	people_list->DeleteItem(people_list->FindItem(-1, name, false));
}

// The append function
void AdminWindow::OnKickEvent(wxSocketEvent& event)
{
	;
}

BEGIN_EVENT_TABLE(AdminWindow, wxFrame)
END_EVENT_TABLE()