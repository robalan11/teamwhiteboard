#include "textWindow.h"
#include "statusWindow.h"
#include "admin.h"

// Defines
#define SERVER_ID 100
#define SOCKET_ID 101
#define TEXT_INPUT_BOX 200
#define CLIENT_OPEN 102
#define CLIENT_CLOSE 103

#define MAX_CLIENTS 64

// Variables that need to be viewed elsewhere

// Strings
wxString name;
wxString ip;
wxString id;

// Networking
bool server;
wxSocketBase *m_server_out[MAX_CLIENTS];
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
AdminWindow *admin;
wxString names[MAX_CLIENTS];
wxString IPs[MAX_CLIENTS];
wxString banlist[128];
int num_banned;

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
		wxPoint(-1, -1), wxSize(-1, -1), wxTE_MULTILINE | wxTE_READONLY | wxTE_RICH);

	hbox2->Add(tc2, 1, wxEXPAND);
	vbox->Add(hbox2, 1, wxLEFT | wxRIGHT | wxEXPAND, 10);

	vbox->Add(-1, 25);

	// Third section, input box
	wxBoxSizer *hbox3 = new wxBoxSizer(wxHORIZONTAL);
	tc3 = new wxTextCtrl(panel, TEXT_INPUT_BOX, wxT(""), 
      wxPoint(-1, -1), wxSize(-1, -1), wxTE_LEFT | wxTE_PROCESS_ENTER);

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

	// Numbers
	num_banned = 0;

	// Create the status window
	status = new StatusWindow("Status");
	status->Show(true);

	//Create the admin window
	admin = new AdminWindow("People");
	admin->Show(true);
	
	// Wrap it up
	panel->SetSizer(vbox);
	Centre();
}

// The quit function
void textWindow::Quit()
{
	status->Close(true);
	admin->Close(true);
	Close(true);
}

// The QUIT event handler
void textWindow::OnQuit(wxCommandEvent& WXUNUSED(event))
{
    Quit();
}

void ParseCommand(wxString line) {
	if (line.Index(' ') + 1 >= line.length() || !line.Index(' ')) {
		tc2->SetDefaultStyle(wxTextAttr(*wxRED));
		tc2->AppendText("Supply some arguments next time.\n");
		tc2->SetDefaultStyle(wxTextAttr(*wxBLACK));
		return;
	}
	wxString command = line.SubString(0, line.Index(' ')-1);
	wxString remainder = line.SubString(line.Index(' ') + 1, line.length()-1);
	
	wxRegEx comparator;
	comparator.Compile("^/me$", 0);
	if (comparator.Matches(command)) {
		wxString output = name + _T(" ") + remainder;
		tc2->AppendText(output);
		if (server){
			// Send it to all clients
			for (int i = 0; i < m_numClients; i++){
				m_server_out[i]->WriteMsg(output.c_str(), (wxStrlen(output) + 1) * sizeof(wxChar));
			}
		}else{
			m_sock_out->WriteMsg(output.c_str(), (wxStrlen(output) + 1) * sizeof(wxChar));
		}
		return;
	}

	comparator.Compile("^/kick$", 0);
	if (server && comparator.Matches(command)) {
		for (int i = 0; i < m_numClients; i++) {
			if (names[i].Cmp(remainder)) {
				m_server_out[i]->WriteMsg("/disconnect", 12 * sizeof(wxChar));
				break;
			}
		}
		return;
	}

	comparator.Compile("^/ban$", 0);
	if (server && comparator.Matches(command)) {
		for (int i = 0; i < m_numClients; i++) {
			if (names[i].Cmp(remainder)) {
				m_server_out[i]->WriteMsg("/disconnect", 12 * sizeof(wxChar));
				banlist[num_banned] = IPs[i];
				num_banned++;
				tc2->SetDefaultStyle(wxTextAttr(*wxRED));
				tc2->AppendText(_T("User ")+names[i]);
				tc2->AppendText(_T(" from IP ")+IPs[i]);
				tc2->AppendText(_T(" has been banned.\n"));
				tc2->SetDefaultStyle(wxTextAttr(*wxBLACK));
				break;
			}
		}
		return;
	}

	comparator.Compile("^/unban$", 0);
	if (server && comparator.Matches(command)) {
		for (int i = 0; i < num_banned; i++) {
			if (banlist[i].Cmp(remainder)) {
				tc2->SetDefaultStyle(wxTextAttr(*wxRED));
				tc2->AppendText(_T("IP ")+banlist[i]);
				tc2->AppendText(_T(" has been unbanned.\n"));
				tc2->SetDefaultStyle(wxTextAttr(*wxBLACK));
				banlist[i] = "";
				break;
			}
		}
		return;
	}

	comparator.Compile("^/roll$", 0);
	if (comparator.Matches(command)) {
		int num = rand()%atoi(remainder) + 1;
		char* str = (char*)malloc(100*sizeof(char));
		sprintf(str, " rolls a d%d and gets a %d.\n", atoi(remainder), num);
		wxString output = name + _T(str);
		free(str);
		tc2->AppendText(output);
		if (server){
			// Send it to all clients
			for (int i = 0; i < m_numClients; i++){
				m_server_out[i]->WriteMsg(output.c_str(), (wxStrlen(output) + 1) * sizeof(wxChar));
			}
		}else{
			m_sock_out->WriteMsg(output.c_str(), (wxStrlen(output) + 1) * sizeof(wxChar));
		}
		return;
	}
	
	tc2->SetDefaultStyle(wxTextAttr(*wxRED));
	tc2->AppendText("Not a valid command.\n");
	tc2->SetDefaultStyle(wxTextAttr(*wxBLACK));
}

// The "text enter" event handler
void textWindow::OnTextEnter(wxCommandEvent& WXUNUSED(event))
{
	wxString input = tc3->GetValue() + _T("\n");
	
	wxRegEx comparator;
	comparator.Compile("^/.+", wxRE_EXTENDED);
	if (comparator.Matches(input)) {
		ParseCommand(input);
		tc3->Clear();
		return;
	}

	// Make sure something is entered
	if (input != "\n"){
		time_t rawtime;
		struct tm * timeinfo;
		char* timestr = (char*)malloc(10*sizeof(char));

		time ( &rawtime );
		timeinfo = localtime ( &rawtime );
		sprintf(timestr, "%02d:%02d:%02d", timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);

		input = name + _T(": ") + input;
		input = _T(") ") + input;
		input = _T(timestr) + input;
		input = _T("(") + input;
		tc2->AppendText(input);
		tc3->Clear();

		free(timestr);

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
	// Find my address!
	wxIPV4address remote;
	remote.Hostname(_T("www.wxwidgets.org"));
	remote.Service(80);
 
	wxIPV4address local;
 
	wxSocketClient client;
	if(client.Connect(remote)) client.GetLocal(local);

	wxIPV4address addr;
	addr.Service(3000);
	addr.Hostname(local.IPAddress());
	status->Append(_T("Server created.  IP: ") + addr.IPAddress() + _T("\n"));

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
	if (m_sock_out->IsConnected()){
		// Send IP
		m_sock_out->WriteMsg(addr.IPAddress().c_str(), (wxStrlen(addr.IPAddress()) + 1) * sizeof(wxChar));
		// Send name
		m_sock_out->WriteMsg(name.c_str(), (wxStrlen(name) + 1) * sizeof(wxChar));
		status->Append(_T("Connection established\n"));
	}else{
		m_sock_out->Close();
		status->Append(_T("Failed ! Unable to connect\n"));
	}
}

// Sets up the server
void textWindow::setupServer()
{
	// Create the address - defaults to localhost:0 initially
	wxIPV4address remote;
	remote.Hostname(_T("www.wxwidgets.org"));
	remote.Service(80);
 
	wxIPV4address local;
 
	wxSocketClient client;
	if(client.Connect(remote)) client.GetLocal(local);

	wxIPV4address addr;
	addr.Service(3000);
	addr.Hostname(local.IPAddress());
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

	// Get the name of the person who connected
	wxChar *IP = new wxChar[16];
	sock->ReadMsg(IP, sizeof(wxChar)*16).LastCount();
	for (int i = 0; i < num_banned; i++){
		if (!strcmp(IP, banlist[i])) {
			sock->WriteMsg("/hellno", 8 * sizeof(wxChar));
			status->Append(_T("Connection from banned IP "));
			status->Append(IP);
			status->Append(_T(" refused.\n"));
			return;
		}
	}

	wxChar *text = new wxChar[100];
	sock->ReadMsg(text, sizeof(wxChar)*100).LastCount();

	// Add the person to the list
	admin->AddPerson(text);

	sock->SetEventHandler(*this, SOCKET_ID);
	sock->SetNotify(wxSOCKET_INPUT_FLAG | wxSOCKET_LOST_FLAG);
	sock->Notify(true);

	status->Append(text);
	status->Append(_T(" has joined\n"));

	m_server_out[m_numClients] = sock;
	names[m_numClients] = text;
	IPs[m_numClients] = IP;
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
				
				if (text[0] == '/') {
					Quit();
				}

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
				// If you're the server, remove the socket from the list
				if (server){
					bool found = false;
					for (int i = 0; i < m_numClients; i++){
						if (found){
							if (i != MAX_CLIENTS - 1){
								m_server_out[i] = m_server_out[i+1];
								names[i] = names[i+1];
							}
						}else{
							if (m_server_out[i] == sock){
								found = true;
								admin->RemovePerson(names[i]);
								status->Append(names[i]);
								status->Append(": Left.\n");
								if (i != MAX_CLIENTS - 1){
									m_server_out[i] = m_server_out[i+1];
									names[i] = names[i+1];
								}
							}
						}
					}
					m_numClients--;
					
				}

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
	EVT_TEXT_ENTER(TEXT_INPUT_BOX, textWindow::OnTextEnter)
	EVT_SOCKET(SERVER_ID,  textWindow::OnServerEvent)
	EVT_SOCKET(SOCKET_ID,  textWindow::OnSocketEvent)
END_EVENT_TABLE()