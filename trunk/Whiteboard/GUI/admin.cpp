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

std::vector<wxString> AdminWindow::getList()
{
	std::vector<wxString> list;
	for (int i = 0; i < people_list->GetItemCount(); i++){
		list.push_back(people_list->GetItemText(i));
	}

	return list;
}

void AdminWindow::updateList(std::vector<wxString> list)
{
	people_list->ClearAll();
	for (unsigned int i = 0; i < list.size(); i++){
		wxListItem item;
		item.SetText(list[i]);
		people_list->InsertItem(item);
	}
}

// The append function
void AdminWindow::OnKickEvent(wxSocketEvent& event)
{
	;
}

BEGIN_EVENT_TABLE(AdminWindow, wxFrame)
END_EVENT_TABLE()