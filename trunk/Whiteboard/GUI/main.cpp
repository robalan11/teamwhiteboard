#include "main.h"
#include "simple.h"
#include "textWindow.h"
#include "prompt.h"

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
	textWindow *window2 = new textWindow(wxT("Chat"));
	Prompt *window = new Prompt(wxT("Enter some info"), window2);
	window2->Show(true);
	window->Show(true);

	return true;
}
