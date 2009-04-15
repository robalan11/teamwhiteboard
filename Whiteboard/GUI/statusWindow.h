#include <wx/wx.h>

class StatusWindow : public wxFrame
{
public:
	StatusWindow(const wxString& title);
	void Append(const wxString& text);
};
