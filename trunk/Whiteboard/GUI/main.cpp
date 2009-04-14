#include "main.h"
#include "simple.h"
#include "textWindow.h"

IMPLEMENT_APP(MyApp)

bool MyApp::OnInit()
{
	textWindow *window = new textWindow(wxT("Chat"));
	window->Show(true);

	return true;
}
