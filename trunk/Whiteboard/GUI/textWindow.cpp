#include "textWindow.h"
#include "statusWindow.h"
#include "admin.h"
#include "whiteboard.h"

// Defines
#define SERVER_ID 100
#define SOCKET_ID 101
#define TEXT_INPUT_BOX 200
#define CLIENT_OPEN 102
#define CLIENT_CLOSE 103

#define MAX_CLIENTS 64

// Variables that need to be viewed elsewhere

// Strings
wxString gName;
wxString gIp;
wxString gId;

// Networking
bool gServer;
wxSocketBase* gpServerOut[MAX_CLIENTS];
wxSocketServer* gpServerIn;
wxSocketClient* gpSockIn;
wxSocketClient* gpSockOut;
bool gBusy;
int  gNumClients;

// Window objects
wxPanel* gpPanel = new wxPanel();
wxTextCtrl* gpTc2;
wxTextCtrl* gpTc3;
wxStaticText* gpHeaderText;
StatusWindow* gpStatus;
AdminWindow* gpAdmin;
WhiteboardWindow* gpWhiteboard;
wxString gNames[MAX_CLIENTS];
wxString gIPs[MAX_CLIENTS];
wxString gBanlist[128];
int gNumBanned;
Prompt* gpPrompt;

// The constructor
TextWindow::TextWindow(const wxString& title)
       : wxFrame(NULL, -1, title, wxPoint(-1, -1), wxSize(300, 400))
{
	// The gpPanel itself
	gpPanel->Create(this, -1);

	wxBoxSizer* vbox = new wxBoxSizer(wxVERTICAL);

	// First section, header
	wxBoxSizer* hbox1 = new wxBoxSizer(wxHORIZONTAL);

	gpHeaderText = new wxStaticText();

	hbox1->Add(gpHeaderText, 0);
	vbox->Add(hbox1, 0, wxLEFT | wxTOP, 10);

	vbox->Add(-1, 10);

	// Second section, text window
	wxBoxSizer* hbox2 = new wxBoxSizer(wxHORIZONTAL);
	gpTc2 = new wxTextCtrl(gpPanel, wxID_ANY, wxT(""), 
		wxPoint(-1, -1), wxSize(-1, -1), wxTE_MULTILINE | wxTE_READONLY | wxTE_RICH);

	hbox2->Add(gpTc2, 1, wxEXPAND);
	vbox->Add(hbox2, 1, wxLEFT | wxRIGHT | wxEXPAND, 10);

	vbox->Add(-1, 25);

	// Third section, input box
	wxBoxSizer* hbox3 = new wxBoxSizer(wxHORIZONTAL);
	gpTc3 = new wxTextCtrl(gpPanel, TEXT_INPUT_BOX, wxT(""), 
      wxPoint(-1, -1), wxSize(-1, -1), wxTE_LEFT | wxTE_PROCESS_ENTER);

	hbox3->Add(gpTc3, 1, wxEXPAND);
	vbox->Add(hbox3, 0, wxLEFT | wxRIGHT | wxEXPAND, 10);

	vbox->Add(-1, 25);

	// Fourth section, buttons
	wxBoxSizer* hbox4 = new wxBoxSizer(wxHORIZONTAL);
	wxButton* btn1 = new wxButton(gpPanel, wxID_SAVE, wxT("Send"));
	hbox4->Add(btn1, 0);
	wxButton* btn2 = new wxButton(gpPanel, wxID_EXIT, wxT("Close"));
	hbox4->Add(btn2, 0, wxLEFT | wxBOTTOM , 5);
	vbox->Add(hbox4, 0, wxALIGN_RIGHT | wxRIGHT, 10);

	// Numbers
	gNumBanned = 0;

	// Create the gpStatus window
	gpStatus = new StatusWindow("Status");
	gpStatus->Show(true);

	//Create the gpAdmin window
	gpAdmin = new AdminWindow("People");
	gpAdmin->Show(true);

	//Create the gpWhiteboard window
	gpWhiteboard = new WhiteboardWindow("Whiteboard", this);
	gpWhiteboard->Show(true);
	
	// Wrap it up
	gpPanel->SetSizer(vbox);
	Centre();
	
	//Create the connection gpPrompt
	gpPrompt = new Prompt(wxT("Enter some info"), this);
	gpPrompt->Show(true);
}

// The quit function
void TextWindow::Quit()
{
	gpStatus->Close(true);
	gpAdmin->Close(true);
	gpWhiteboard->Close(true);
	Close(true);
}

// The QUIT event handler
void TextWindow::OnQuit(wxCommandEvent& WXUNUSED(event))
{
    Quit();
}

void ParseCommand(wxString line) {
	if (line.Index(' ') + 1 >= line.length() || !line.Index(' ')) {
		gpTc2->SetDefaultStyle(wxTextAttr(*wxRED));
		gpTc2->AppendText("Supply some arguments next time.\n");
		gpTc2->SetDefaultStyle(wxTextAttr(*wxBLACK));
		return;
	}
	wxString command = line.SubString(0, line.Index(' ')-1);
	wxString remainder = line.SubString(line.Index(' ') + 1, line.length()-1);
	
	wxRegEx comparator;
	comparator.Compile("^/me$", 0);
	if (comparator.Matches(command)) {
		wxString output = gName + _T(" ") + remainder;
		gpTc2->AppendText(output);
		if (gServer){
			// Send it to all clients
			for (int i = 0; i < gNumClients; i++){
				gpServerOut[i]->WriteMsg(output.c_str(), (wxStrlen(output) + 1) * sizeof(wxChar));
			}
		}else{
			gpSockOut->WriteMsg(output.c_str(), (wxStrlen(output) + 1) * sizeof(wxChar));
		}
		return;
	}

	comparator.Compile("^/kick$", 0);
	if (gServer && comparator.Matches(command)) {
		for (int i = 0; i < gNumClients; i++) {
			if (gNames[i].Cmp(remainder) == 0) {
				gpStatus->Append("REMANDER: " + remainder + "\n");
				gpStatus->Append("NAME: " + gNames[i] + "\n");
				gpServerOut[i]->WriteMsg("/disconnect", 12 * sizeof(wxChar));
				break;
			}
		}
		return;
	}

	comparator.Compile("^/ban$", 0);
	if (gServer && comparator.Matches(command)) {
		for (int i = 0; i < gNumClients; i++) {
			if (gNames[i].Cmp(remainder) == 0) {
				gpServerOut[i]->WriteMsg("/disconnect", 12 * sizeof(wxChar));
				gBanlist[gNumBanned] = gIPs[i];
				gNumBanned++;
				gpTc2->SetDefaultStyle(wxTextAttr(*wxRED));
				gpTc2->AppendText(_T("User ")+gNames[i]);
				gpTc2->AppendText(_T(" from IP ")+gIPs[i]);
				gpTc2->AppendText(_T(" has been banned.\n"));
				gpTc2->SetDefaultStyle(wxTextAttr(*wxBLACK));
				break;
			}
		}
		return;
	}

	comparator.Compile("^/unban$", 0);
	if (gServer && comparator.Matches(command)) {
		for (int i = 0; i < gNumBanned; i++) {
			if (gBanlist[i].Cmp(remainder)) {
				gpTc2->SetDefaultStyle(wxTextAttr(*wxRED));
				gpTc2->AppendText(_T("IP ")+gBanlist[i]);
				gpTc2->AppendText(_T(" has been unbanned.\n"));
				gpTc2->SetDefaultStyle(wxTextAttr(*wxBLACK));
				gBanlist[i] = "";
				gNumBanned--;
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
		wxString output = gName + _T(str);
		free(str);
		gpTc2->AppendText(output);
		if (gServer){
			// Send it to all clients
			for (int i = 0; i < gNumClients; i++){
				gpServerOut[i]->WriteMsg(output.c_str(), (wxStrlen(output) + 1) * sizeof(wxChar));
			}
		}else{
			gpSockOut->WriteMsg(output.c_str(), (wxStrlen(output) + 1) * sizeof(wxChar));
		}
		return;
	}
	
	gpTc2->SetDefaultStyle(wxTextAttr(*wxRED));
	gpTc2->AppendText("Not a valgId command.\n");
	gpTc2->SetDefaultStyle(wxTextAttr(*wxBLACK));
}

// The "text enter" event handler
void TextWindow::OnTextEnter(wxCommandEvent& WXUNUSED(event))
{
	wxString input = gpTc3->GetValue();	
	wxRegEx comparator;
	comparator.Compile("^/.+", wxRE_EXTENDED);
	if (comparator.Matches(input)) {
		ParseCommand(input);
		gpTc3->Clear();
		return;
	}

	// Make sure something is entered
	if (input != ""){
		time_t rawtime;
		struct tm * timeinfo;
		char* timestr = (char*)malloc(10*sizeof(char));

		time ( &rawtime );
		timeinfo = localtime ( &rawtime );
		sprintf(timestr, "%02d:%02d:%02d", timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);

		input = gName + _T(": ") + input;
		input = _T(") ") + input;
		input = _T(timestr) + input;
		input = _T("(") + input;
		gpTc2->AppendText(input);
		gpTc3->Clear();

		free(timestr);

		// Send it over the network
		if (gServer){
			// Send it to all clients
			for (int i = 0; i < gNumClients; i++){
				gpServerOut[i]->WriteMsg(input.c_str(), (wxStrlen(input) + 1) * sizeof(wxChar));
			}
		}else{
			gpSockOut->WriteMsg(input.c_str(), (wxStrlen(input) + 1) * sizeof(wxChar));
		}
	}
}

// Sets up a client
void TextWindow::SetupClient()
{
	// Find my address!
	wxIPV4address remote;
	remote.Hostname(_T("www.wxwgIdgets.org"));
	remote.Service(80);
 
	wxIPV4address local;
 
	wxSocketClient client;
	if(client.Connect(remote)) client.GetLocal(local);

	wxIPV4address addr;
	addr.Service(3000);
	addr.Hostname(local.IPAddress());

	// Create the socket
	gpSockOut = new wxSocketClient();

	// Setup the event handler and subscribe to most events
	gpSockOut->SetEventHandler(*this, SOCKET_ID);
	gpSockOut->SetNotify(wxSOCKET_CONNECTION_FLAG |
					  wxSOCKET_INPUT_FLAG |
					  wxSOCKET_LOST_FLAG);
	gpSockOut->Notify(true);

	gBusy = false;

	// Connect to the gServer
	wxIPV4address server_ip;
	server_ip.Hostname(gIp);
	server_ip.Service(3000);
	gpStatus->Append(_T("==Connecting to gServer (") + gIp + _T(")==\n"));
	gpSockOut->Connect(server_ip, false);
	gpSockOut->WaitOnConnect(10);
	
	// Test connection
	if (gpSockOut->IsConnected()){
		// Send IP
		gpSockOut->WriteMsg(addr.IPAddress().c_str(), (wxStrlen(addr.IPAddress()) + 1) * sizeof(wxChar));
		// See if you're banned :-)
		wxChar* text = new wxChar[16];
		gpSockOut->ReadMsg(text, sizeof(wxChar)*10000).LastCount();
		wxString test(text);
		if (test == "/disconnect"){
			gpSockOut->Close();
			gpPrompt->Close(true);
			Quit();
		}

		// Send gName
		gpSockOut->WriteMsg(gName.c_str(), (wxStrlen(gName) + 1) * sizeof(wxChar));
		gpStatus->Append(_T("Connection established\n"));
		// Get list of connected people
		std::vector<wxString> list;
		while (1){
			wxChar *text = new wxChar[100];
			gpSockOut->ReadMsg(text, sizeof(wxChar) * 100).LastCount();
			wxString test(text);
			if (test != _T("/done"))
				list.push_back(test);
			else
				break;
		}
		gpAdmin->UpdateList(list);

		// Get gpWhiteboard
		std::vector< std::vector<wxString> > objects;
		std::vector<wxString> shape;
		wxString message;
		while (1){
			wxChar *text = new wxChar[50];
			gpSockOut->ReadMsg(text, sizeof(wxChar) * 50).LastCount();
			wxString test(text);
			if (test != _T("/done")){
				shape.push_back(test);
				gpSockOut->ReadMsg(text, sizeof(wxChar) * 50).LastCount();
				shape.push_back((wxString)text);
				gpSockOut->ReadMsg(text, sizeof(wxChar) * 50).LastCount();
				shape.push_back((wxString)text);
				gpSockOut->ReadMsg(text, sizeof(wxChar) * 50).LastCount();
				shape.push_back((wxString)text);
				gpSockOut->ReadMsg(text, sizeof(wxChar) * 50).LastCount();
				shape.push_back((wxString)text);
				objects.push_back(shape);
				shape.clear();
			}else
				break;
		}
		if (objects.size() != 0){
			gpWhiteboard->mpCanvas->objects = objects;
			gpWhiteboard->mpCanvas->Refresh();
		}
	}else{
		gpSockOut->Close();
		gpStatus->Append(_T("Failed ! Unable to connect\n"));
	}
}

// Sets up the gServer
void TextWindow::SetupServer()
{
	// Create the address - defaults to localhost:0 initially
	wxIPV4address remote;
	remote.Hostname(_T("www.wxwgIdgets.org"));
	remote.Service(80);
 
	wxIPV4address local;
 
	wxSocketClient client;
	if(client.Connect(remote)) client.GetLocal(local);

	wxIPV4address addr;
	addr.Service(3000);
	addr.Hostname(local.IPAddress());
	gpStatus->Append(_T("Server created.  IP: ") + addr.IPAddress() + _T("\n"));

	// Create the socket
	gpServerIn = new wxSocketServer(addr);

	// We use Ok() here to see if the gServer is really listening
	if (! gpServerIn->Ok()){
		gpStatus->Append(_T("Could not listen at the specified port !\n\n"));
		return;
	}else{
		gpStatus->Append(_T("Server listening.\n\n"));
	}

	// Setup the event handler and subscribe to connection events
	gpServerIn->SetEventHandler(*this, SERVER_ID);
	gpServerIn->SetNotify(wxSOCKET_CONNECTION_FLAG);
	gpServerIn->Notify(true);

	gBusy = false;
	gNumClients = 0;
}

// Initializes the window
void TextWindow::Initialize(wxString n, wxString i)
{
	// Modify the window
	gName = n;
	gIp = i;
	wxString text = "Name: " + gName + "     Address: " + gIp;
	gpHeaderText->Create(gpPanel, wxID_ANY, 
      text);

	// Setup the gServer
	if (gIp == _T("none")){
		gServer = true;
		SetupServer();
	}else{
		gServer = false;
		SetupClient();
	}
}

void TextWindow::SendNewShape(std::vector<wxString> shape)
{
	// Send it for everyone else
	wxString intro(_T("/NewShape"));
	wxString done(_T("/done"));
	for (int i = 0; i < gNumClients; i++){
		gpServerOut[i]->WriteMsg(intro.c_str(), (wxStrlen(intro) + 1) * sizeof(wxChar));
		for (unsigned int j = 0; j < shape.size(); j++){
			gpServerOut[i]->WriteMsg(shape[j].c_str(), (wxStrlen(shape[j]) + 1) * sizeof(wxChar));
		}
		gpServerOut[i]->WriteMsg(done.c_str(), (wxStrlen(done) + 1) * sizeof(wxChar));
	}
}

// Server event handler
void TextWindow::OnServerEvent(wxSocketEvent &WXUNUSED(event))
{
	wxSocketBase* sock;

	// Accept new connection if there is one in the pending
	// connections queue, else exit. We use Accept(false) for
	// non-blocking accept (although if we got here, there
	// should ALWAYS be a pending connection).

	sock = gpServerIn->Accept(false);

	if (gpServerOut){
		gpStatus->Append(_T("New client connection accepted\n\n"));
	}else{
		gpStatus->Append(_T("Error: couldn't accept a new connection\n\n"));
		return;
	}

	// Get the gName of the person who connected
	wxChar* ip = new wxChar[16];
	sock->ReadMsg(ip, sizeof(wxChar)*16).LastCount();
	for (int i = 0; i < gNumBanned; i++){
		if (!strcmp(ip, gBanlist[i])) {
			sock->WriteMsg("/disconnect", 12 * sizeof(wxChar));
			gpStatus->Append(_T("Connection from banned IP "));
			gpStatus->Append(ip);
			gpStatus->Append(_T(" refused.\n"));
			return;
		}
	}
	sock->WriteMsg("/welcome", 9 * sizeof(wxChar));

	wxChar* text = new wxChar[100];
	sock->ReadMsg(text, sizeof(wxChar)*100).LastCount();

	// Add the person to the list
	gpAdmin->AddPerson(text);

	// Send the list of people already connected
	std::vector<wxString> list = gpAdmin->GetList();
	for (unsigned int i = 0; i < list.size(); i++){
		sock->WriteMsg(list[i].c_str(), (wxStrlen(list[i]) + 1) * sizeof(wxChar));
	}
	wxString done(_T("/done"));
	sock->WriteMsg(done.c_str(), (wxStrlen(done) + 1) * sizeof(wxChar));

	// Send it for everyone else
	wxString intro(_T("/beginList"));
	for (int i = 0; i < gNumClients; i++){
		gpServerOut[i]->WriteMsg(intro.c_str(), (wxStrlen(intro) + 1) * sizeof(wxChar));
		for (unsigned int j = 0; j < list.size(); j++){
			gpServerOut[i]->WriteMsg(list[j].c_str(), (wxStrlen(list[j]) + 1) * sizeof(wxChar));
		}
		gpServerOut[i]->WriteMsg(done.c_str(), (wxStrlen(done) + 1) * sizeof(wxChar));
	}
	
	// Send the entire friggin gpWhiteboard.  Sheesh.  I mean, come on
	for (unsigned int i = 0; i < gpWhiteboard->mpCanvas->objects.size(); i++){
		std::vector<wxString> shape = gpWhiteboard->mpCanvas->objects[i];
		for (unsigned int j = 0; j < shape.size(); j++){
			sock->WriteMsg(shape[j].c_str(), (wxStrlen(shape[j]) + 1) * sizeof(wxChar));
		}
	}
	sock->WriteMsg(done.c_str(), (wxStrlen(done) + 1) * sizeof(wxChar));

	sock->SetEventHandler(*this, SOCKET_ID);
	sock->SetNotify(wxSOCKET_INPUT_FLAG | wxSOCKET_LOST_FLAG);
	sock->Notify(true);

	gpStatus->Append(text);
	gpStatus->Append(_T(" has joined\n"));

	gpServerOut[gNumClients] = sock;
	gNames[gNumClients] = text;
	gIPs[gNumClients] = ip;
	gNumClients++;
}

// Socket event handler
void TextWindow::OnSocketEvent(wxSocketEvent& event)
{
		wxSocketBase* sock = event.GetSocket();
		wxChar* text = new wxChar[10000];

		// Now we process the event
		switch(event.GetSocketEvent()){
			case wxSOCKET_INPUT:
			{
				// We disable input events, so that the test doesn't trigger
				// wxSocketEvent again.
				sock->SetNotify(wxSOCKET_LOST_FLAG);

				// Get the text from the socket
				sock->ReadMsg(text, sizeof(wxChar)*10000).LastCount();

				wxString test(text);
				if (test == "/beginList"){
					// Get list of connected people
					std::vector<wxString> list;
					while (1){
						wxChar* text2 = new wxChar[100];
						sock->ReadMsg(text2, sizeof(wxChar) * 100).LastCount();
						wxString test2(text2);
						if (test2 != _T("/done"))
							list.push_back(test2);
						else
							break;
					}
					gpAdmin->UpdateList(list);
				}else if (test == "/NewShape"){
					// Get the new shape
					std::vector<wxString> shape;
					while (1){
						wxChar* text2 = new wxChar[100];
						sock->ReadMsg(text2, sizeof(wxChar) * 100).LastCount();
						wxString test2(text2);
						if (test2 != _T("/done"))
							shape.push_back(test2);
						else
							break;
					}
					if (shape[0] == _T("clear")){
						gpWhiteboard->mpCanvas->objects.clear();
						gpWhiteboard->mpCanvas->Refresh();
					}else if (shape[0] == _T("undo")) {
						gpWhiteboard->mpCanvas->objects.pop_back();
						gpWhiteboard->mpCanvas->Refresh();
					}else{
						gpWhiteboard->mpCanvas->AddNewShape(shape);
					}
				}else{

					if (text[0] == '/') {
						Quit();
					}

					// Display it on the window
					gpTc2->AppendText(text);

					// Send the messages to the other clients
					for (int i = 0; i < gNumClients; i++){
						if (gpServerOut[i] != sock)
							gpServerOut[i]->WriteMsg(text, (wxStrlen(text) + 1) * sizeof(wxChar));
					}
				}
				// Enable input events again.
				sock->SetNotify(wxSOCKET_LOST_FLAG | wxSOCKET_INPUT_FLAG);
				break;
			}
			case wxSOCKET_LOST:
			{
				// If you're the gServer, remove the socket from the list
				if (gServer){
					bool found = false;
					for (int i = 0; i < gNumClients; i++){
						if (found){
							if (i != MAX_CLIENTS - 1){
								gpServerOut[i] = gpServerOut[i+1];
								gNames[i] = gNames[i+1];
							}
						}else{
							if (gpServerOut[i] == sock){
								found = true;
								gpAdmin->RemovePerson(gNames[i]);
								gpStatus->Append(gNames[i]);
								gpStatus->Append(": Left.\n");
								if (i != MAX_CLIENTS - 1){
									gpServerOut[i] = gpServerOut[i+1];
									gNames[i] = gNames[i+1];
								}
							}
						}
					}
					gNumClients--;

					// Send it for everyone else
					wxString intro(_T("/beginList"));
					wxString done(_T("/done"));
					std::vector<wxString> list = gpAdmin->GetList();
					for (int i = 0; i < gNumClients; i++){
						gpServerOut[i]->WriteMsg(intro.c_str(), (wxStrlen(intro) + 1) * sizeof(wxChar));
						for (unsigned int j = 0; j < list.size(); j++){
							gpServerOut[i]->WriteMsg(list[j].c_str(), (wxStrlen(list[j]) + 1) * sizeof(wxChar));
						}
						gpServerOut[i]->WriteMsg(done.c_str(), (wxStrlen(done) + 1) * sizeof(wxChar));
					}
				}

				// Destroy() should be used instead of delete wherever possible,
				// due to the fact that wxSocket uses 'delayed events' (see the
				// documentation for wxPostEvent) and we don't want an event to
				// arrive to the event handler (the frame, here) after the socket
				// has been deleted. Also, we might be doing some other thing with
				// the socket at the same time; for example, we might be in the
				// mgIddle of a test or something. Destroy() takes care of all
				// this for us.

				gpStatus->Append(_T("Deleting socket.\n\n"));
				sock->Destroy();
				break;
			}
			default: ;
		}
}

bool TextWindow::IsServer()
{
	return gServer;
}

// The event table
BEGIN_EVENT_TABLE(TextWindow, wxFrame)
	EVT_BUTTON(wxID_EXIT,  TextWindow::OnQuit)
	EVT_BUTTON(wxID_SAVE,  TextWindow::OnTextEnter)
	EVT_TEXT_ENTER(TEXT_INPUT_BOX, TextWindow::OnTextEnter)
	EVT_SOCKET(SERVER_ID,  TextWindow::OnServerEvent)
	EVT_SOCKET(SOCKET_ID,  TextWindow::OnSocketEvent)
END_EVENT_TABLE()