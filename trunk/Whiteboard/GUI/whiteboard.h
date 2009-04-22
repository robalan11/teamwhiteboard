#ifndef __WHITEBOARD
#define __WHITEBOARD

#include <wx/wx.h>
#include <wx/socket.h>
#include <wx/listctrl.h>
#include <wx/window.h>
#include <wx/scrolwin.h>
#include <wx/dcclient.h>
#include <wx/button.h>
#include <math.h>
#include <algorithm>
#include "textWindow.h"
#include <vector>
#include <stdlib.h>

class WhiteboardWindow;
class TextWindow;

/**
 * This is the Canvas!  It's where stuff that is drawn is displayed.
 */

class MyCanvas: public wxWindow
{
public:
    MyCanvas( WhiteboardWindow *parent );

	/**
	 * Sets the start position when the left mouse button is pressed
	 */
	void SetStartPos(wxMouseEvent &event);

	/**
	 * Draw the right thing based on selected tool, start position,
	 * and current position.
	 */
	void SetEndPos(wxMouseEvent &event);

	/**
	 * Callback function that is run when the window is redrawn
	 */
    void OnPaint(wxPaintEvent &event);

	/**
	 * Callback function that is run when the mouse is moved
	 */
    void OnMouseMove(wxMouseEvent &event);

	/**
	 * Function that adds a shape to the collection of shapes
	 */
	void AddNewShape(std::vector<wxString> shape);

	long         mX;
	long         mY;
	wxPoint      mStartPos;
	std::vector<std::vector<wxString> > objects;

private:
    WhiteboardWindow *mpOwner;

    wxBitmap     mSmileBmp;
    wxIcon       mStdIcon;
    bool         mClip;
	bool         mDrawing;

    DECLARE_EVENT_TABLE()
};

/**
 * This is the whiteboard window.  The TA can draw in it!
 */

class WhiteboardWindow : public wxFrame
{
	public:

		/**
		 * The constructor
		 */
		WhiteboardWindow(const wxString& title, TextWindow *parent);

		/**
		 * Prepares the dc
		 */
		void PrepareDC(wxDC& dc);

		/**
		 * Tool functions.  They each set the active tool and change the statusbar message.
		 */
		void Clear(wxCommandEvent &WXUNUSED(event));
		void Line(wxCommandEvent &WXUNUSED(event));
		void Rect(wxCommandEvent &WXUNUSED(event));
		void Circ(wxCommandEvent &WXUNUSED(event));
		//void Free(wxCommandEvent &WXUNUSED(event));
		void Undo(wxCommandEvent &WXUNUSED(event));
		
		int         mBackgroundMode;
		int         mTextureBackground;
		int         mMapMode;
		double      mXUserScale;
		double      mYUserScale;
		int         mXLogicalOrigin;
		int         mYLogicalOrigin;
		bool        mXAxisReversed,
					mYAxisReversed;
		wxColour    mColourForeground,    // these are _text_ colours
			        mColourBackground;
		wxBrush     mBackgroundBrush;
	    MyCanvas   *mpCanvas;

		wxString    mActiveTool;
		TextWindow *mpParent;

	private:

		/**
		 * The macro for the event table.
		 */
		DECLARE_EVENT_TABLE()
};

#endif