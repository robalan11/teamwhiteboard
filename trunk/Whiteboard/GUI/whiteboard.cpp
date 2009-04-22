#include "whiteboard.h"

WhiteboardWindow::WhiteboardWindow(const wxString& title, TextWindow* parent)
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
	wxButton* butt_clear = new wxButton();
	butt_clear->Create(this, 0, "Clear", wxDefaultPosition, wxSize(32,32));
	wxButton* butt_line = new wxButton();
	butt_line->Create(this, 1, "Line", wxPoint(0,32), wxSize(32,32));
	wxButton* butt_rect = new wxButton();
	butt_rect->Create(this, 2, "Rect", wxPoint(0,64), wxSize(32,32));
	wxButton* butt_circ = new wxButton();
	butt_circ->Create(this, 3, "Circle", wxPoint(0,96), wxSize(32,32));
	//wxButton *butt_free = new wxButton();
	//butt_free->Create(this, 4, "Free\nDraw", wxPoint(0,128), wxSize(32,32));
	wxButton* butt_undo = new wxButton();
	butt_undo->Create(this, 5, "Undo", wxPoint(0,128), wxSize(32,32));

}

void WhiteboardWindow::PrepareDC(wxDC& dc)
{
    dc.SetLogicalOrigin( mXLogicalOrigin, mYLogicalOrigin );
    dc.SetAxisOrientation( !mXAxisReversed, mYAxisReversed );
    dc.SetUserScale( mXUserScale, mYUserScale );
    dc.SetMapMode( mMapMode );
}

void WhiteboardWindow::Clear(wxCommandEvent& WXUNUSED(event))
{
	SetStatusText(_T("Clear"));
	mActiveTool = "clear";
}

void WhiteboardWindow::Line(wxCommandEvent& WXUNUSED(event))
{
	SetStatusText(_T("Click and drag to draw a line."));
	mActiveTool = "line";
}

void WhiteboardWindow::Rect(wxCommandEvent& WXUNUSED(event))
{
	SetStatusText(_T("Click and drag to draw a rectangle."));
	mActiveTool = "rect";
}

void WhiteboardWindow::Circ(wxCommandEvent& WXUNUSED(event))
{
	SetStatusText(_T("Click and drag to draw a circle."));
	mActiveTool = "circ";
}

/*void WhiteboardWindow::Free(wxCommandEvent& WXUNUSED(event))
{
	SetStatusText(_T("Click and drag to draw freely."));
	mActiveTool = "free";
}*/

void WhiteboardWindow::Undo(wxCommandEvent& WXUNUSED(event))
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