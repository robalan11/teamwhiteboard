#include <wx/wx.h>
#include <wx/socket.h>
#include <wx/listctrl.h>

/**
 * This is the admin window.  The TA can perform administrative
 * tasks on the students.
 */

class AdminWindow : public wxFrame
{
	public:

		/**
		 * The constructor
		 */
		AdminWindow(const wxString& title);

		/**
		 * Event handler for kick
		 */
		void OnKickEvent(wxSocketEvent& event);

	private:

		/**
		 * The macro for the event table.
		 */
		DECLARE_EVENT_TABLE()
};
