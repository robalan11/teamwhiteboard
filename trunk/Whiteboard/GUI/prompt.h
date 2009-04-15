#include <wx/wx.h>

class Prompt : public wxFrame
{
public:
	Prompt(const wxString& title, wxFrame *parent);
	void OnOK(wxCommandEvent& WXUNUSED(event));
	void OnCancel(wxCommandEvent& WXUNUSED(event));
	void ChangeIPField(wxCommandEvent& WXUNUSED(event));

private:
    DECLARE_EVENT_TABLE()
};
