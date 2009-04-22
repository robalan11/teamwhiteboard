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
		 * Removes a person from the list of people.
		 */
		void RemovePerson(wxString name);
		
		/**
		 * Returns the whole freaking list.
		 */
		std::vector<wxString> GetList();
		
		/**
		 * Adds the items in the given vector to the list
		 */
		void UpdateList(std::vector<wxString> list);

	private:

		wxListCtrl* mpPeopleList;	// The list of people connected
};
