#include <wx/wx.h>

class textWindow : public wxFrame
{
  public:
    textWindow(const wxString& title);
	void OnQuit(wxCommandEvent& event);
	void OnTextEnter(wxCommandEvent& event);
	void initialize(wxString n, wxString i);

  private:
    DECLARE_EVENT_TABLE()

};
