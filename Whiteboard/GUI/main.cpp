#include "main.h"
#include "simple.h"
#include "textWindow.h"

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
	textWindow *window = new textWindow(wxT("Chat"));
	window->Show(true);

	return true;
}
