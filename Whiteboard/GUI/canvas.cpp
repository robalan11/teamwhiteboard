#include "canvas.h"	// ISA canvas

MyCanvas::MyCanvas(WhiteboardWindow* parent) : wxWindow(parent, wxID_ANY, wxPoint(32,0), wxSize(602,431))
{
	mpOwner = parent;
    mClip = false;
	mDrawing = false;
}

void MyCanvas::OnPaint(wxPaintEvent& WXUNUSED(event))
{
    wxPaintDC pdc(this);
    wxDC& dc = pdc ;

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

void MyCanvas::SetStartPos(wxMouseEvent& WXUNUSED(event))
{
	if (mpOwner->mpParent->IsServer()) {
		mStartPos = wxPoint(mX, mY);
		mDrawing = true;
	}
}

void MyCanvas::SetEndPos(wxMouseEvent& WXUNUSED(event))
{
	if (mpOwner->mpParent->IsServer()) {
		if (mDrawing) {
		    Refresh();
			mDrawing = false;
		}
	}
}

BEGIN_EVENT_TABLE(MyCanvas, wxWindow)
	EVT_PAINT  (MyCanvas::OnPaint)
    EVT_MOTION (MyCanvas::OnMouseMove)
	
	//Mouse events
	EVT_LEFT_DOWN(MyCanvas::SetStartPos)
	EVT_LEFT_UP(MyCanvas::SetEndPos)
END_EVENT_TABLE()