#include "build/PrecompiledHeaders.h"
#include <iostream>

#ifdef _WINDOWS
#include <windows.h>
#endif // _WINDOWS

#include "render/RenderWindow.h"
#include "render/RenderThreadContext.h"
#include "boost/scoped_ptr.hpp"


// -----------------------------------------------------------------------------


using namespace std;
using namespace boost;


// -----------------------------------------------------------------------------


int main(int argc, char* argv[])
{
	cout << "Hello world." << endl;

	RenderWindow renderWindow;
	
	renderWindow.Init();

	MSG msg;
	{
		scoped_ptr<RenderThreadContext> renderThreadContext(renderWindow.CreateRenderThreadContext());

		while(GetMessage(&msg, NULL, 0, 0) > 0)
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	renderWindow.Shutdown();

	return msg.wParam;
}