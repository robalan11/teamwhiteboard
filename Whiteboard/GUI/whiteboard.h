#ifndef WHITEBOARD
#define WHITEBOARD

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
#include "canvas.h"
#include <vector>
#include <stdlib.h>

class MyCanvas;
class TextWindow;


/**
 * This is the whiteboard window.  The TA can draw in it!
 */

class WhiteboardWindow : public wxFrame
{
	public:

		/**
		 * The constructor
		 */
		WhiteboardWindow(const wxString& title, TextWindow* parent);

		/**
		 * Prepares the dc for drawing
		 */
		void PrepareDC(wxDC& dc);

		/**
		 * Tool functions.  They each set the active tool and change the statusbar message.
		 */
		void Clear(wxCommandEvent& WXUNUSED(event));
		void Line(wxCommandEvent& WXUNUSED(event));
		void Rect(wxCommandEvent& WXUNUSED(event));
		void Circ(wxCommandEvent& WXUNUSED(event));
		//void Free(wxCommandEvent& WXUNUSED(event));
		void Undo(wxCommandEvent& WXUNUSED(event));
		
		int         mBackgroundMode;			// Mode of the background
		int         mTextureBackground;			// Texture of the background
		int         mMapMode;					// Mapping mode
		double      mXUserScale;				// X scale
		double      mYUserScale;				// Y scale
		int         mXLogicalOrigin;			// X origin
		int         mYLogicalOrigin;			// Y origin
		bool        mXAxisReversed,				// Is the x axis reversed?
					mYAxisReversed;				// Is the y axis reversed?
		wxColour    mColourForeground,			// these are _text_ colours
			        mColourBackground;			// Background color
		wxBrush     mBackgroundBrush;			// Background brush
	    MyCanvas*   mpCanvas;					// The canvas in this window

		wxString    mActiveTool;				// The currently active tool
		TextWindow* mpParent;					// The text window (parent to this window)

	private:

		/**
		 * The macro for the event table.
		 */
		DECLARE_EVENT_TABLE()
};

#endif