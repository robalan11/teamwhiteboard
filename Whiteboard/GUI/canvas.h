#ifndef CANVAS
#define CANVAS

#include <wx/wx.h>
#include <wx/socket.h>
#include <wx/listctrl.h>
#include <wx/window.h>
#include <wx/scrolwin.h>
#include <wx/dcclient.h>
#include <wx/button.h>
#include <math.h>
#include <algorithm>
#include "whiteboard.h"
#include <vector>
#include <stdlib.h>

class WhiteboardWindow;

/**
 * This is the Canvas.  It is where all the shapes that have been drawn
 * are displayed.
 */

class MyCanvas: public wxWindow
{
public:
    MyCanvas( WhiteboardWindow* parent );

	/**
	 * Sets the start position of the shape that is currently being drawn
	 * when the left mouse button is pressed.
	 */
	void SetStartPos(wxMouseEvent& event);

	/**
	 * Draws the shape that the currently selected tool draws based on
	 * start position and current mouse position.
	 */
	void SetEndPos(wxMouseEvent& event);

	/**
	 * Callback function that is run whenever the window is redrawn.
	 */
    void OnPaint(wxPaintEvent& event);

	/**
	 * Callback function that is run when the mouse is moved.
	 */
    void OnMouseMove(wxMouseEvent& event);

	/**
	 * Function that adds a shape to the collection of shapes.
	 */
	void AddNewShape(std::vector<wxString> shape);

	long         mX;								// Current x position
	long         mY;								// Current y position
	wxPoint      mStartPos;							// Start position of a shape
	std::vector<std::vector<wxString> > objects;	// The list of objects

private:
    WhiteboardWindow *mpOwner;						// The window the canvas is in

    bool         mClip;								// Determines clipping
	bool         mDrawing;							// Flag of whether or not a shape is currently being drawn

    DECLARE_EVENT_TABLE()                           // Event Table macro
};

#endif