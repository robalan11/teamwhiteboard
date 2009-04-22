#include <wx/wx.h>			// USES lots of things (wxWidgets main file)
#include <wx/socket.h>		// USES socket class
#include <wx/listctrl.h>	// HASA list control
#include <vector>			// USES vectors

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
