#include <wx/wx.h>

/**
 * This is the status window.  It's a white box that displays messages related 
 * to the program's status.
 */

class StatusWindow : public wxFrame
{
public:

	/**
	 * The constructor
	 */
	StatusWindow(const wxString& title);

	/**
	 * Appends a new line of text to the status window.
	 */
	void Append(const wxString& text);

	wxTextCtrl* mpTextBox;	// The text box into which status text is placed
};
