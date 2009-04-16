#ifndef __PROMPT
#define __PROMPT
#include <wx/wx.h>

/**
 * This is the prompt before the chat window becomes enabled.  The user is 
 * asked for their name, whether the computer is a client or server, and IP to 
 * connecto to (client only).
 */

class Prompt : public wxFrame
{
public:

	/**
	 * The constructor.
	 */
	Prompt(const wxString& title, wxFrame *parent);

	/**
	 * Event handler for the OK button.  It checks to make sure all the 
	 * information is valid, and enables the chat window.
	 */
	void OnOK(wxCommandEvent& WXUNUSED(event));

	/**
	 * Event handler for the CANCEL button.  Discards all entered information, 
	 * and closes the windows.
	 */
	void OnCancel(wxCommandEvent& WXUNUSED(event));

	/**
	 * Event handler for the combo box.  Enables/Disables the IP question 
	 * depending on whether the user entered client or server.
	 */
	void ChangeIPField(wxCommandEvent& WXUNUSED(event));

private:

	/**
	 * Macro for the event handler.
	 */
    DECLARE_EVENT_TABLE()
};

#endif
