#ifndef TEXT_WINDOW
#define TEXT_WINDOW

#include <wx/wx.h>		// USES lots of things (wxWidgets main file)
#include <wx/socket.h>	// USES socket class
#include <wx/menu.h>	// HASA menu
#include <wx/regex.h>	// USES regular expressions
#include <time.h>		// USES time
#include <stdlib.h>		// USES various functions
#include <string>		// USES strings
#include <vector>		// USES vectors
#include "prompt.h"		// HASA prompt window

/**
 * This is the chat window.  The user can enter and send text, like with any 
 * other chat window.  This window also handles the status window for system 
 * messages.
 */

class TextWindow : public wxFrame
{
	public:

		/**
		 * The constructor
		 */
		TextWindow(const wxString& title);

		/**
		 * Shuts down the chat window and the status window.
		 */
		void Quit();

		/**
		 * Event handler for the Close button.  Simply calls Quit.
		 */
		void OnQuit(wxCommandEvent& event);

		/**
		 * Event handler for the Send button.  First, it displays the entered text in 
		 * the text box.  Then, it sends the entered text to the server.
		 */
		void OnTextEnter(wxCommandEvent& event);

		/**
		 * Initializes the window.  It sets up either a client or a server, modifies 
		 * the header and enables the window.
		 */
		void Initialize(wxString n, wxString i);

		/**
		 * Event handler for sockets.  When a socket receives input, it's displayed 
		 * in the chat window.
		 */
		void OnSocketEvent(wxSocketEvent& event);

		/**
		 * Event handler for server events.  When a server receives a 
		 * connection, it accepts and creates another socket for IO.
		 */
		void OnServerEvent(wxSocketEvent& event);

		/**
		 * Returns whether or not I am a server
		 */
		bool IsServer();

		/**
		 * Sends a new shape to all connected clients
		 */
		void SendNewShape(std::vector<wxString>);

	private:

		/**
		 * Sets up a client.  It attempts to connect to the IP provided by the 
		 * user.
		 */
		void SetupClient();

		/**
		 * Sets up a server.  The server starts listening for connections on 
		 * localhost.
		 */
		void SetupServer();

		/**
		 * The macro for the event table.
		 */
		DECLARE_EVENT_TABLE()
};

#endif