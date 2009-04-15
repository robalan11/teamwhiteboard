#include <wx/wx.h>
#include <wx/socket.h>

class textWindow : public wxFrame
{
	public:
		textWindow(const wxString& title);
		void Quit();
		void OnQuit(wxCommandEvent& event);
		void OnTextEnter(wxCommandEvent& event);
		void initialize(wxString n, wxString i);
		void OnSocketEvent(wxSocketEvent& event);
		void OnServerEvent(wxSocketEvent& event);

	private:
		void setupClient();
		void setupServer();
		DECLARE_EVENT_TABLE()

};
