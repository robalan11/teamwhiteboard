#include <wx/wx.h>
#include <wx/socket.h>
#include <wx/listctrl.h>
#include <vector>

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
		 * Adds a person to the list of people.
		 */
		void AddPerson(wxString name);
		
		/**
		 * Removes a sheep.
		 */
		void RemovePerson(wxString name);
		
		/**
		 * Returns the whole freaking list.
		 */
		std::vector<wxString> getList();
		
		/**
		 * Adds the items in the given vector to the list
		 */
		void updateList(std::vector<wxString> list);

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
