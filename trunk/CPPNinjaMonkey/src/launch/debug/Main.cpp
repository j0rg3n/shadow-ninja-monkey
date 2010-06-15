#include <iostream>

#ifdef _WINDOWS
#include <windows.h>
#endif // _WINDOWS

#include "render/RenderWindow.h"
#include "RenderWorker.h"

#include "boost/scoped_ptr.hpp"
#include "boost/bind.hpp"

#include <GL.h>
#include <GLU.h>


// -----------------------------------------------------------------------------


using namespace std;
using namespace boost;


// -----------------------------------------------------------------------------


class App
{
public:
	void Init()
	{
		m_renderWindow.Init();

		m_renderWindow.SizeChanged().connect(bind(&App::OnSizeChanged, this, _1, _2));
		m_renderWindow.Closed().connect(bind(&App::OnClosed, this));

		// Create worker before calling OnSizeChanged, as the worker internally
		// creates an OpenGL render context for this thread.
		// TODO: Solve this encapsulation issue.
		m_pWorker.reset(new RenderWorker(m_renderWindow));

		OnSizeChanged(m_renderWindow.Width(), m_renderWindow.Height());
	}


	void Shutdown()
	{
		m_pWorker.reset();
		m_renderWindow.Shutdown();
	}


	WPARAM MessageLoop()
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
				OnIdle();
			}
		}
	}

private:
	void OnIdle()
	{
		// TODO: Generalize with nice pool of worker threads and a barrier-style
		// gizmo for letting them all complete before the swap, and restart after 
		// the swap.

		// This requires more knowledge about the main game logic thread, so
		// postponing for now.
		m_pWorker->Run();
		m_renderWindow.Swap();
	}


	void OnSizeChanged(int width, int height)
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


	void OnClosed()
	{
		PostQuitMessage(0);
	}


	RenderWindow m_renderWindow;
	scoped_ptr<RenderWorker> m_pWorker;
};


// -----------------------------------------------------------------------------


int main(int argc, char* argv[])
{
	cout << "Hello world." << endl;

	App app;

	app.Init();

	WPARAM exitCode = app.MessageLoop();

	app.Shutdown();

	return exitCode;
}