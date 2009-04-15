#include "textWindow.h"
#include "statusWindow.h"

// Defines
#define SERVER_ID 100
#define SOCKET_ID 101
#define CLIENT_OPEN 102
#define CLIENT_CLOSE 103

// Variables that need to be viewed elsewhere

// Strings
wxString name;
wxString ip;
wxString id;

// Networking
bool server;
wxSocketBase *m_server_out[64];
wxSocketServer *m_server_in;
wxSocketClient *m_sock_in;
wxSocketClient *m_sock_out;
bool m_busy;
int m_numClients;

// Window objects
wxPanel *panel = new wxPanel();
wxTextCtrl *tc2;
wxTextCtrl *tc3;
wxStaticText *headerText;
StatusWindow *status;

// The constructor
textWindow::textWindow(const wxString& title)
       : wxFrame(NULL, -1, title, wxPoint(-1, -1), wxSize(300, 400))
{

	// The panel itself
	panel->Create(this, -1);

	wxBoxSizer *vbox = new wxBoxSizer(wxVERTICAL);

	// First section, header
	wxBoxSizer *hbox1 = new wxBoxSizer(wxHORIZONTAL);

	headerText = new wxStaticText();

	hbox1->Add(headerText, 0);
	vbox->Add(hbox1, 0, wxLEFT | wxTOP, 10);

	vbox->Add(-1, 10);

	// Second section, text window
	wxBoxSizer *hbox2 = new wxBoxSizer(wxHORIZONTAL);
	tc2 = new wxTextCtrl(panel, wxID_ANY, wxT(""), 
		wxPoint(-1, -1), wxSize(-1, -1), wxTE_MULTILINE | wxTE_READONLY);

	hbox2->Add(tc2, 1, wxEXPAND);
	vbox->Add(hbox2, 1, wxLEFT | wxRIGHT | wxEXPAND, 10);

	vbox->Add(-1, 25);

	// Third section, input box
	wxBoxSizer *hbox3 = new wxBoxSizer(wxHORIZONTAL);
	tc3 = new wxTextCtrl(panel, 1, wxT(""), 
      wxPoint(-1, -1), wxSize(-1, -1), wxTE_LEFT);

	hbox3->Add(tc3, 1, wxEXPAND);
	vbox->Add(hbox3, 0, wxLEFT | wxRIGHT | wxEXPAND, 10);

	vbox->Add(-1, 25);

	// Fourth section, buttons
	wxBoxSizer *hbox4 = new wxBoxSizer(wxHORIZONTAL);
	wxButton *btn1 = new wxButton(panel, wxID_SAVE, wxT("Send"));
	hbox4->Add(btn1, 0);
	wxButton *btn2 = new wxButton(panel, wxID_EXIT, wxT("Close"));
	hbox4->Add(btn2, 0, wxLEFT | wxBOTTOM , 5);
	vbox->Add(hbox4, 0, wxALIGN_RIGHT | wxRIGHT, 10);

	// Create the status window
	status = new StatusWindow("Status");
	status->Show(true);
	
	// Wrap it up
	panel->SetSizer(vbox);
	Centre();
}

// The quit function
void textWindow::Quit()
{
	status->Close(true);
	Close(true);
}

// The QUIT event handler
void textWindow::OnQuit(wxCommandEvent& WXUNUSED(event))
{
    Quit();
}

// The "text enter" event handler
void textWindow::OnTextEnter(wxCommandEvent& WXUNUSED(event))
{
	wxString input = tc3->GetValue() + _T("\n");

	// Make sure something is entered
	if (input != "\n"){
		input = name + _T(": ") + input;
		tc2->AppendText(input);
		tc3->Clear();

		// Send it over the network
		if (server){
			// Send it to all clients
			for (int i = 0; i < m_numClients; i++){
				m_server_out[i]->WriteMsg(input.c_str(), (wxStrlen(input) + 1) * sizeof(wxChar));
			}
		}else{
			m_sock_out->WriteMsg(input.c_str(), (wxStrlen(input) + 1) * sizeof(wxChar));
		}
	}
}

// Sets up a client
void textWindow::setupClient()
{
	// Create the socket
	m_sock_out = new wxSocketClient();

	// Setup the event handler and subscribe to most events
	m_sock_out->SetEventHandler(*this, SOCKET_ID);
	m_sock_out->SetNotify(wxSOCKET_CONNECTION_FLAG |
					  wxSOCKET_INPUT_FLAG |
					  wxSOCKET_LOST_FLAG);
	m_sock_out->Notify(true);

	m_busy = false;

	// Connect to the server
	wxIPV4address server_ip;
	server_ip.Hostname(ip);
	server_ip.Service(3000);
	status->Append(_T("==Connecting to server (") + ip + _T(")==\n"));
	m_sock_out->Connect(server_ip, false);
	m_sock_out->WaitOnConnect(10);
	
	// Test connection
	if (m_sock_out->IsConnected())
		status->Append(_T("Connection established\n"));
	else{
		m_sock_out->Close();
		status->Append(_T("Failed ! Unable to connect\n"));
	}
}

// Sets up the server
void textWindow::setupServer()
{
	// Create the address - defaults to localhost:0 initially
	wxIPV4address addr;
	addr.Service(3000);
	status->Append(_T("Server created.  IP: ") + addr.IPAddress() + _T("\n"));

	// Create the socket
	m_server_in = new wxSocketServer(addr);

	// We use Ok() here to see if the server is really listening
	if (! m_server_in->Ok()){
		status->Append(_T("Could not listen at the specified port !\n\n"));
		return;
	}else{
		status->Append(_T("Server listening.\n\n"));
	}

	// Setup the event handler and subscribe to connection events
	m_server_in->SetEventHandler(*this, SERVER_ID);
	m_server_in->SetNotify(wxSOCKET_CONNECTION_FLAG);
	m_server_in->Notify(true);

	m_busy = false;
	m_numClients = 0;
}

// Initializes the window
void textWindow::initialize(wxString n, wxString i)
{
	// Modify the window
	name = n;
	ip = i;
	wxString text = "Name: " + name + "     Address: " + ip;
	headerText->Create(panel, wxID_ANY, 
      text);

	// Setup the server
	if (ip == _T("none")){
		server = true;
		setupServer();
	}else{
		server = false;
		setupClient();
	}
}

// Server event handler
void textWindow::OnServerEvent(wxSocketEvent& event)
{
	wxSocketBase *sock;

	// Accept new connection if there is one in the pending
	// connections queue, else exit. We use Accept(false) for
	// non-blocking accept (although if we got here, there
	// should ALWAYS be a pending connection).

	sock = m_server_in->Accept(false);

	if (m_server_out){
		status->Append(_T("New client connection accepted\n\n"));
	}else{
		status->Append(_T("Error: couldn't accept a new connection\n\n"));
		return;
	}

	sock->SetEventHandler(*this, SOCKET_ID);
	sock->SetNotify(wxSOCKET_INPUT_FLAG | wxSOCKET_LOST_FLAG);
	sock->Notify(true);

	m_server_out[m_numClients] = sock;
	m_numClients++;
}

// Socket event handler
void textWindow::OnSocketEvent(wxSocketEvent& event)
{
		wxSocketBase *sock = event.GetSocket();
		wxChar *text = new wxChar[10000];

		// Now we process the event
		switch(event.GetSocketEvent()){
			case wxSOCKET_INPUT:
			{
				// We disable input events, so that the test doesn't trigger
				// wxSocketEvent again.
				sock->SetNotify(wxSOCKET_LOST_FLAG);

				// Get the text from the socket
				sock->ReadMsg(text, sizeof(wxChar)*10000).LastCount();

				// Display it on the window
				tc2->AppendText(text);

				// Send the messages to the other clients
				for (int i = 0; i < m_numClients; i++){
					if (m_server_out[i] != sock)
						m_server_out[i]->WriteMsg(text, (wxStrlen(text) + 1) * sizeof(wxChar));
				}

				// Enable input events again.
				sock->SetNotify(wxSOCKET_LOST_FLAG | wxSOCKET_INPUT_FLAG);
				break;
			}
			case wxSOCKET_LOST:
			{
			m_numClients--;

			// Destroy() should be used instead of delete wherever possible,
			// due to the fact that wxSocket uses 'delayed events' (see the
			// documentation for wxPostEvent) and we don't want an event to
			// arrive to the event handler (the frame, here) after the socket
			// has been deleted. Also, we might be doing some other thing with
			// the socket at the same time; for example, we might be in the
			// middle of a test or something. Destroy() takes care of all
			// this for us.

			status->Append(_T("Deleting socket.\n\n"));
			sock->Destroy();
			break;
			}
			default: ;
		}
}

// The event table
BEGIN_EVENT_TABLE(textWindow, wxFrame)
	EVT_BUTTON(wxID_EXIT,  textWindow::OnQuit)
	EVT_BUTTON(wxID_SAVE, textWindow::OnTextEnter)
	EVT_TEXT_ENTER(1, textWindow::OnTextEnter)
	EVT_SOCKET(SERVER_ID,  textWindow::OnServerEvent)
	EVT_SOCKET(SOCKET_ID,  textWindow::OnSocketEvent)
END_EVENT_TABLE()