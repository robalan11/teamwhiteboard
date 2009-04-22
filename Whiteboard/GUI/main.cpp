#include "main.h"		// ISA main
#include "textWindow.h"	// HASA text window

//Thank you to Onega Zhang (Google search'd) for this fix.
#include <wx/apptrait.h>
#if wxUSE_STACKWALKER && defined( __WXDEBUG__ )
wxString wxAppTraitsBase::GetAssertStackTrace()
{
   return wxT("");
}
#endif

IMPLEMENT_APP(MyApp)

bool MyApp::OnInit()
{
	TextWindow* window = new TextWindow(wxT("Chat"));
	window->Show(true);

	return true;
}
