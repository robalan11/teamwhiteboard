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

/**
 * This is the Canvas!  It's where stuff goes!
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

    void OnPaint(wxPaintEvent &event);
    void OnMouseMove(wxMouseEvent &event);

	void addNewShape(std::vector<wxString> shape);

	long         x;
	long         y;
	wxPoint      m_startPos;
	std::vector<std::vector<wxString> > objects;

private:
    WhiteboardWindow *m_owner;

    wxBitmap     m_smile_bmp;
    wxIcon       m_std_icon;
    bool         m_clip;

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
		WhiteboardWindow(const wxString& title, textWindow *parent);

		/**
		 * Prepares the dc
		 */
		void PrepareDC(wxDC& dc);

		/**
		 * Tool functions
		 */
		void Clear(wxCommandEvent &WXUNUSED(event));
		void Line(wxCommandEvent &WXUNUSED(event));
		void Rect(wxCommandEvent &WXUNUSED(event));
		void Circ(wxCommandEvent &WXUNUSED(event));
		void Free(wxCommandEvent &WXUNUSED(event));
		void Undo(wxCommandEvent &WXUNUSED(event));
		
		int         m_backgroundMode;
		int         m_textureBackground;
		int         m_mapMode;
		double      m_xUserScale;
		double      m_yUserScale;
		int         m_xLogicalOrigin;
		int         m_yLogicalOrigin;
		bool        m_xAxisReversed,
					m_yAxisReversed;
		wxColour    m_colourForeground,    // these are _text_ colours
			        m_colourBackground;
		wxBrush     m_backgroundBrush;
	    MyCanvas   *m_canvas;

		wxString    m_activeTool;
		textWindow *m_parent;

	private:

		/**
		 * The macro for the event table.
		 */
		DECLARE_EVENT_TABLE()
};