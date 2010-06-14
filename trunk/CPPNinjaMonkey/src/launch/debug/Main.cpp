#include <iostream>

#ifdef _WINDOWS
#include <windows.h>
#endif // _WINDOWS

#include "render/RenderWindow.h"
#include "RenderWorker.h"
#include "boost/scoped_ptr.hpp"


#include <GL.h>
#include <GLU.h>


// -----------------------------------------------------------------------------


using namespace std;
using namespace boost;


// -----------------------------------------------------------------------------


static WPARAM MessageLoop(RenderWorker &worker, RenderWindow& renderWindow)
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
			worker.Run();
			renderWindow.Swap();
		}
	}
}


static void OnSizeChanged(int width, int height)
{
    if (height == 0)
    {
        height = 1;
    }

	// TODO: Move away: Perspective matrix should be set in render code, not here.
    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(54.0f, (GLfloat) width / (GLfloat) height, 1.0f, 1000.0f);
}


static void OnClosed()
{
	PostQuitMessage(0);
}


// -----------------------------------------------------------------------------


int main(int argc, char* argv[])
{
	cout << "Hello world." << endl;

	RenderWindow renderWindow;
	renderWindow.Init();

	renderWindow.SizeChanged().connect(OnSizeChanged);
	renderWindow.Closed().connect(OnClosed);

	WPARAM exitCode;
	{
		RenderWorker worker(renderWindow);
		OnSizeChanged(renderWindow.Width(), renderWindow.Height());

		exitCode = MessageLoop(worker, renderWindow);
	}

	renderWindow.Shutdown();

	return exitCode;
}