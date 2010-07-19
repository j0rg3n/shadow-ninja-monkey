#include "diag/Trace.h"

#include "../App.h"

#include <windows.h>

#include "boost/scoped_ptr.hpp"


// -----------------------------------------------------------------------------


using namespace boost;


// -----------------------------------------------------------------------------


static WPARAM MessageLoop(App& app)
{
	while(true)
	{
		MSG msg;
		BOOL bResult = PeekMessage(&msg, NULL, 0, 0, PM_REMOVE);
		if(bResult == TRUE)
		{
			if(msg.message == WM_QUIT)
			{
				return msg.wParam;
			}
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			app.OnIdle();
		}
	}
}


static void OnClosed()
{
	TRACE("Window closed.");
	PostQuitMessage(0);
}


// -----------------------------------------------------------------------------


int main(int argc, char* argv[])
{
	scoped_ptr<App> pApp(App::CreateInstance());

	pApp->Init();

	pApp->ConnectClosedSlot(OnClosed);

	WPARAM exitCode = MessageLoop(*pApp);

	pApp->Shutdown();

	return exitCode;
}