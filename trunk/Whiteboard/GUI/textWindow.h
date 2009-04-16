#include <wx/wx.h>
#include <wx/socket.h>
#include <wx/menu.h>
#include <wx/regex.h>

/**
 * This is the chat window.  The user can enter and send text, like with any 
 * other chat window.  This window also handles the status window for system 
 * messages.
 */

class textWindow : public wxFrame
{
	public:

		/**
		 * The constructor
		 */
		textWindow(const wxString& title);

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
		void initialize(wxString n, wxString i);

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

	private:

		/**
		 * Sets up a client.  It attempts to connect to the IP provided by the 
		 * user.
		 */
		void setupClient();

		/**
		 * Sets up a server.  The server starts listening for connections on 
		 * localhost.
		 */
		void setupServer();

		/**
		 * The macro for the event table.
		 */
		DECLARE_EVENT_TABLE()
};
