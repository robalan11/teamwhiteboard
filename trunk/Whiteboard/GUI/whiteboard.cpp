#include "whiteboard.h"

WhiteboardWindow::WhiteboardWindow(const wxString& title, TextWindow *parent)
       : wxFrame(NULL, -1, title, wxPoint(-1, -1), wxSize(640, 480), wxMINIMIZE_BOX | wxSYSTEM_MENU | wxCAPTION | wxCLIP_CHILDREN)
{	
	mpParent = parent;

    CreateStatusBar(2);
    SetStatusText(_T("Welcome to the Whiteboard!"));

	mMapMode = wxMM_TEXT;
    mXUserScale = 1.0;
    mYUserScale = 1.0;
    mXLogicalOrigin = 0;
    mYLogicalOrigin = 0;
    mXAxisReversed =
    mYAxisReversed = false;
    mBackgroundMode = wxSOLID;
    mColourForeground = *wxBLACK;
    mColourBackground = *wxWHITE;
    mTextureBackground = false;

	mActiveTool = "";

    mpCanvas = new MyCanvas( this );
	mpCanvas->Show();

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
    dc.SetLogicalOrigin( mXLogicalOrigin, mYLogicalOrigin );
    dc.SetAxisOrientation( !mXAxisReversed, mYAxisReversed );
    dc.SetUserScale( mXUserScale, mYUserScale );
    dc.SetMapMode( mMapMode );
}

void WhiteboardWindow::Clear(wxCommandEvent &WXUNUSED(event))
{
	SetStatusText(_T("Clear"));
	mActiveTool = "clear";
}

void WhiteboardWindow::Line(wxCommandEvent &WXUNUSED(event))
{
	SetStatusText(_T("Click and drag to draw a line."));
	mActiveTool = "line";
}

void WhiteboardWindow::Rect(wxCommandEvent &WXUNUSED(event))
{
	SetStatusText(_T("Click and drag to draw a rectangle."));
	mActiveTool = "rect";
}

void WhiteboardWindow::Circ(wxCommandEvent &WXUNUSED(event))
{
	SetStatusText(_T("Click and drag to draw a circle."));
	mActiveTool = "circ";
}

/*void WhiteboardWindow::Free(wxCommandEvent &WXUNUSED(event))
{
	SetStatusText(_T("Click and drag to draw freely."));
	mActiveTool = "free";
}*/

void WhiteboardWindow::Undo(wxCommandEvent &WXUNUSED(event))
{
	if (mpParent->IsServer()) {
		if (mpCanvas->objects.size() > 0) {
			SetStatusText(_T("Last command undone."));
			mActiveTool = "undo";
			mpCanvas->objects.pop_back();
			mpCanvas->Refresh();
		}
		else {
			mActiveTool = "";
		}
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
	mpOwner = parent;
    mClip = false;
	mDrawing = false;
}

void MyCanvas::OnPaint(wxPaintEvent &WXUNUSED(event))
{
    wxPaintDC pdc(this);
    wxDC &dc = pdc ;

    mpOwner->PrepareDC(dc);
	
	std::vector<wxString> foob; //foob!@
	char buffer[128];
	if (mpOwner->mpParent->IsServer()){
		if (mpOwner->mActiveTool == "") return;

		if (mpOwner->mActiveTool == "clear")
		{
			objects.clear();
			foob.push_back(_T("clear"));
		}

		if (mpOwner->mActiveTool == "line")
		{
			foob.push_back(_T("line"));
			sprintf(buffer, "%d", mStartPos.x);
			foob.push_back(_T(buffer));
			sprintf(buffer, "%d", mStartPos.y);
			foob.push_back(_T(buffer));
			sprintf(buffer, "%d", mX);
			foob.push_back(_T(buffer));
			sprintf(buffer, "%d", mY);
			foob.push_back(_T(buffer));
		}

		if (mpOwner->mActiveTool == "rect")
		{
			foob.push_back(_T("rect"));
			sprintf(buffer, "%d", mStartPos.x);
			foob.push_back(_T(buffer));
			sprintf(buffer, "%d", mStartPos.y);
			foob.push_back(_T(buffer));
			sprintf(buffer, "%d", mX-mStartPos.x);
			foob.push_back(_T(buffer));
			sprintf(buffer, "%d", mY-mStartPos.y);
			foob.push_back(_T(buffer));
		}

		if (mpOwner->mActiveTool == "circ")
		{
			foob.push_back(_T("circ"));
			sprintf(buffer, "%d", mStartPos.x);
			foob.push_back(_T(buffer));
			sprintf(buffer, "%d", mStartPos.y);
			foob.push_back(_T(buffer));
			sprintf(buffer, "%d", mX-mStartPos.x);
			foob.push_back(_T(buffer));
			sprintf(buffer, "%d", mY-mStartPos.y);
			foob.push_back(_T(buffer));
		}

		if (mpOwner->mActiveTool == "free")
		{
			dc.SetPen( wxPen( wxT("black"), 1, wxSOLID) );
			dc.DrawLine( mStartPos.x, mStartPos.y, mX, mY );
		}

		if (mpOwner->mActiveTool == "undo")
		{
			foob.push_back(_T("undo"));
		}

		if (mpOwner->mActiveTool != "clear" && mpOwner->mActiveTool != "undo") objects.push_back(foob);
		
		mpOwner->mpParent->SendNewShape(foob);
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

void MyCanvas::AddNewShape(std::vector<wxString> shape)
{
	wxString temp = mpOwner->mActiveTool;
	mpOwner->mActiveTool = shape[0];
	objects.push_back(shape);
	Refresh();
	//mpOwner->mActiveTool = temp;
}

void MyCanvas::OnMouseMove(wxMouseEvent &event)
{
    wxClientDC dc(this);
    PrepareDC(dc);
    mpOwner->PrepareDC(dc);

    wxPoint pos = event.GetPosition();
    mX = dc.DeviceToLogicalX( pos.x );
    mY = dc.DeviceToLogicalY( pos.y );
    wxString str;
    str.Printf( wxT("Current mouse position: %d,%d"), (int)mX, (int)mY );
    mpOwner->SetStatusText( str );
}

void MyCanvas::SetStartPos(wxMouseEvent &WXUNUSED(event))
{
	if (mpOwner->mpParent->IsServer()) {
		mStartPos = wxPoint(mX, mY);
		mDrawing = true;
	}
}

void MyCanvas::SetEndPos(wxMouseEvent &WXUNUSED(event))
{
	if (mpOwner->mpParent->IsServer()) {
		if (mDrawing) {
		    Refresh();
			mDrawing = false;
		}
	}
}