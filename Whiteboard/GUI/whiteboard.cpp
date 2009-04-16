#include "whiteboard.h"

WhiteboardWindow::WhiteboardWindow(const wxString& title, textWindow *parent)
       : wxFrame(NULL, -1, title, wxPoint(-1, -1), wxSize(640, 480), wxMINIMIZE_BOX | wxSYSTEM_MENU | wxCAPTION | wxCLIP_CHILDREN)
{	
	m_parent = parent;

    CreateStatusBar(2);
    SetStatusText(_T("Welcome to the Whiteboard!"));

	m_mapMode = wxMM_TEXT;
    m_xUserScale = 1.0;
    m_yUserScale = 1.0;
    m_xLogicalOrigin = 0;
    m_yLogicalOrigin = 0;
    m_xAxisReversed =
    m_yAxisReversed = false;
    m_backgroundMode = wxSOLID;
    m_colourForeground = *wxBLACK;
    m_colourBackground = *wxWHITE;
    m_textureBackground = false;

	m_activeTool = "";

    m_canvas = new MyCanvas( this );
	m_canvas->Show();

	//Buttons!
	wxButton *butt_clear = new wxButton();
	butt_clear->Create(this, 0, "Clear", wxDefaultPosition, wxSize(32,32));
	wxButton *butt_line = new wxButton();
	butt_line->Create(this, 1, "Line", wxPoint(0,32), wxSize(32,32));
	wxButton *butt_rect = new wxButton();
	butt_rect->Create(this, 2, "Rect", wxPoint(0,64), wxSize(32,32));
	wxButton *butt_circ = new wxButton();
	butt_circ->Create(this, 3, "Circle", wxPoint(0,96), wxSize(32,32));
	//wxButton *butt_free = new wxButton();
	//butt_free->Create(this, 4, "Free\nDraw", wxPoint(0,128), wxSize(32,32));
	wxButton *butt_undo = new wxButton();
	butt_undo->Create(this, 5, "Undo", wxPoint(0,128), wxSize(32,32));

}

void WhiteboardWindow::PrepareDC(wxDC& dc)
{
    dc.SetLogicalOrigin( m_xLogicalOrigin, m_yLogicalOrigin );
    dc.SetAxisOrientation( !m_xAxisReversed, m_yAxisReversed );
    dc.SetUserScale( m_xUserScale, m_yUserScale );
    dc.SetMapMode( m_mapMode );
}

void WhiteboardWindow::Clear(wxCommandEvent &WXUNUSED(event))
{
	SetStatusText(_T("Clear"));
	m_activeTool = "clear";
}

void WhiteboardWindow::Line(wxCommandEvent &WXUNUSED(event))
{
	SetStatusText(_T("Click and drag to draw a line."));
	m_activeTool = "line";
}

void WhiteboardWindow::Rect(wxCommandEvent &WXUNUSED(event))
{
	SetStatusText(_T("Click and drag to draw a rectangle."));
	m_activeTool = "rect";
}

void WhiteboardWindow::Circ(wxCommandEvent &WXUNUSED(event))
{
	SetStatusText(_T("Click and drag to draw a circle."));
	m_activeTool = "circ";
}

/*void WhiteboardWindow::Free(wxCommandEvent &WXUNUSED(event))
{
	SetStatusText(_T("Click and drag to draw freely."));
	m_activeTool = "free";
}*/

void WhiteboardWindow::Undo(wxCommandEvent &WXUNUSED(event))
{
	if (m_canvas->objects.size() > 0) {
		SetStatusText(_T("Last command undone."));
		m_activeTool = "undo";
		m_canvas->objects.pop_back();
		m_canvas->Refresh();
	}
	else {
		m_activeTool = "";
	}
}

BEGIN_EVENT_TABLE(WhiteboardWindow, wxFrame)
	//Buttons
	EVT_BUTTON (0, WhiteboardWindow::Clear)
	EVT_BUTTON (1, WhiteboardWindow::Line)
	EVT_BUTTON (2, WhiteboardWindow::Rect)
	EVT_BUTTON (3, WhiteboardWindow::Circ)
	//EVT_BUTTON (4, WhiteboardWindow::Free)
	EVT_BUTTON (5, WhiteboardWindow::Undo)
END_EVENT_TABLE()


BEGIN_EVENT_TABLE(MyCanvas, wxWindow)
	EVT_PAINT  (MyCanvas::OnPaint)
    EVT_MOTION (MyCanvas::OnMouseMove)
	
	//Mouse events
	EVT_LEFT_DOWN(MyCanvas::SetStartPos)
	EVT_LEFT_UP(MyCanvas::SetEndPos)
END_EVENT_TABLE()

MyCanvas::MyCanvas(WhiteboardWindow *parent) : wxWindow(parent, wxID_ANY, wxPoint(32,0), wxSize(602,431))
{
	m_owner = parent;
    m_clip = false;
	drawing = false;
}

void MyCanvas::OnPaint(wxPaintEvent &WXUNUSED(event))
{
    wxPaintDC pdc(this);
    wxDC &dc = pdc ;

    m_owner->PrepareDC(dc);
	
	std::vector<wxString> foob; //foob!@
	char buffer[128];
	if (m_owner->m_parent->IsServer()){
		if (m_owner->m_activeTool == "") return;

		if (m_owner->m_activeTool == "clear")
		{
			objects.clear();
			foob.push_back(_T("clear"));
		}

		if (m_owner->m_activeTool == "line")
		{
			foob.push_back(_T("line"));
			sprintf(buffer, "%d", m_startPos.x);
			foob.push_back(_T(buffer));
			sprintf(buffer, "%d", m_startPos.y);
			foob.push_back(_T(buffer));
			sprintf(buffer, "%d", x);
			foob.push_back(_T(buffer));
			sprintf(buffer, "%d", y);
			foob.push_back(_T(buffer));
		}

		if (m_owner->m_activeTool == "rect")
		{
			foob.push_back(_T("rect"));
			sprintf(buffer, "%d", m_startPos.x);
			foob.push_back(_T(buffer));
			sprintf(buffer, "%d", m_startPos.y);
			foob.push_back(_T(buffer));
			sprintf(buffer, "%d", x-m_startPos.x);
			foob.push_back(_T(buffer));
			sprintf(buffer, "%d", y-m_startPos.y);
			foob.push_back(_T(buffer));
		}

		if (m_owner->m_activeTool == "circ")
		{
			foob.push_back(_T("circ"));
			sprintf(buffer, "%d", m_startPos.x);
			foob.push_back(_T(buffer));
			sprintf(buffer, "%d", m_startPos.y);
			foob.push_back(_T(buffer));
			sprintf(buffer, "%d", x-m_startPos.x);
			foob.push_back(_T(buffer));
			sprintf(buffer, "%d", y-m_startPos.y);
			foob.push_back(_T(buffer));
		}

		if (m_owner->m_activeTool == "free")
		{
			dc.SetPen( wxPen( wxT("black"), 1, wxSOLID) );
			dc.DrawLine( m_startPos.x, m_startPos.y, x, y );
		}

		if (m_owner->m_activeTool == "undo")
		{
			foob.push_back(_T("undo"));
		}

		if (m_owner->m_activeTool != "clear" && m_owner->m_activeTool != "undo") objects.push_back(foob);
		
		m_owner->m_parent->sendNewShape(foob);
	}

	for (unsigned int i = 0; i < objects.size(); i++)
	{
		dc.SetPen( wxPen( wxT("black"), 1, wxSOLID) );
		dc.SetBrush( *wxGREY_BRUSH );
		if (objects[i][0] == "line") {
			dc.DrawLine( atoi(objects[i][1]), atoi(objects[i][2]), atoi(objects[i][3]), atoi(objects[i][4]) );
		}
		else if (objects[i][0] == "rect") {
			dc.DrawRectangle( atoi(objects[i][1]), atoi(objects[i][2]), atoi(objects[i][3]), atoi(objects[i][4]) );
		}
		else if (objects[i][0] == "circ") {
			dc.DrawEllipse( atoi(objects[i][1]), atoi(objects[i][2]), atoi(objects[i][3]), atoi(objects[i][4]) );
		}

	}
}

void MyCanvas::addNewShape(std::vector<wxString> shape)
{
	wxString temp = m_owner->m_activeTool;
	m_owner->m_activeTool = shape[0];
	objects.push_back(shape);
	Refresh();
	//m_owner->m_activeTool = temp;
}

void MyCanvas::OnMouseMove(wxMouseEvent &event)
{
    wxClientDC dc(this);
    PrepareDC(dc);
    m_owner->PrepareDC(dc);

    wxPoint pos = event.GetPosition();
    x = dc.DeviceToLogicalX( pos.x );
    y = dc.DeviceToLogicalY( pos.y );
    wxString str;
    str.Printf( wxT("Current mouse position: %d,%d"), (int)x, (int)y );
    m_owner->SetStatusText( str );
}

void MyCanvas::SetStartPos(wxMouseEvent &WXUNUSED(event))
{
	if (m_owner->m_parent->IsServer()) {
		m_startPos = wxPoint(x, y);
		drawing = true;
	}
}

void MyCanvas::SetEndPos(wxMouseEvent &WXUNUSED(event))
{
	if (m_owner->m_parent->IsServer()) {
		if (drawing) {
		    Refresh();
			drawing = false;
		}
	}
}