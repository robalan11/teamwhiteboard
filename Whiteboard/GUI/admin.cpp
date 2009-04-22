#include "admin.h"

AdminWindow::AdminWindow(const wxString& title)
       : wxFrame(NULL, -1, title, wxPoint(-1, -1), wxSize(300, 300))
{
	// Add the people box
	mpPeopleList = new wxListCtrl();
	mpPeopleList->Create(this, -1, wxDefaultPosition, wxDefaultSize, wxLC_LIST|wxLC_SORT_ASCENDING|wxLC_SINGLE_SEL|wxLC_HRULES);
	
}

void AdminWindow::AddPerson(wxString name)
{
	wxListItem* new_item = new wxListItem();
	new_item->SetText(name);
	mpPeopleList->InsertItem(*new_item);
}

void AdminWindow::RemovePerson(wxString name)
{
	mpPeopleList->DeleteItem(mpPeopleList->FindItem(-1, name, false));
}

std::vector<wxString> AdminWindow::GetList()
{
	std::vector<wxString> list;
	for (int i = 0; i < mpPeopleList->GetItemCount(); i++){
		list.push_back(mpPeopleList->GetItemText(i));
	}

	return list;
}

void AdminWindow::UpdateList(std::vector<wxString> list)
{
	mpPeopleList->ClearAll();
	for (unsigned int i = 0; i < list.size(); i++){
		wxListItem item;
		item.SetText(list[i]);
		mpPeopleList->InsertItem(item);
	}
}