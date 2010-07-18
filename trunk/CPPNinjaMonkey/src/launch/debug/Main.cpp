#include <iostream>

#ifdef _WINDOWS
#include <windows.h>
#endif // _WINDOWS

#include "input/win32/WindowsMessageInput.h"
#include "render/RenderWindow.h"
#include "RenderWorker.h"

#include "diag/Trace.h"

#include "boost/scoped_ptr.hpp"
#include "boost/bind.hpp"

#include "net/PeerServer.h"
#include "lib/net/Socket.h"
#include "lib/game/GameLoop.h"
#include "lib/game/GameNetworkPacketTranslator.h"
#include "framework/DispatchThread.h"

#include <gl/GL.h>
#include <gl/GLU.h> //< gluPerspective


// -----------------------------------------------------------------------------


using namespace std;
using namespace boost;


// -----------------------------------------------------------------------------


class App
{
public:
	App() : m_gameNetworkPacketTranslator(m_gameLoop)
	{
	}


	void Init()
	{
		TRACE("Starting debug launcher...");
		InitRender();
		InitNetwork();
		InitInput();
		TRACE("Debug launcher started.");
	}


	void Shutdown()
	{
		TRACE("Shutting down debug launcher...");
		ShutdownInput();
		ShutdownNetwork();
		ShutdownRender();
		TRACE("Debug launcher shut down.");
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
		TRACE("Updating projection for new viewport size %dx%d", width, height);
		glViewport(0, 0, width, height);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(54.0f, (GLfloat) width / (GLfloat) height, 1.0f, 1000.0f);
	}


	void OnClosed()
	{
		TRACE("Window closed.");
		PostQuitMessage(0);
	}


	void InitInput()	
	{
		m_pWindowsMessageInput.reset(CreateWindowsMessageInput(&m_renderWindow));
	}


	void ShutdownInput()	
	{
		m_pWindowsMessageInput.reset();
	}


	void InitRender()
	{
		m_renderWindow.Init();

		m_renderWindow.ConnectSizeChangedSlot(boost::bind(&App::OnSizeChanged, this, _1, _2));
		m_renderWindow.ConnectClosedSlot(boost::bind(&App::OnClosed, this));

		// Create worker before calling OnSizeChanged, as the worker internally
		// creates an OpenGL render context for this thread.
		// TODO: Solve this encapsulation issue.
		m_pWorker.reset(new RenderWorker(m_renderWindow));

		OnSizeChanged(m_renderWindow.Width(), m_renderWindow.Height());
	}


	void ShutdownRender()
	{
		m_pWorker.reset();
		m_renderWindow.Shutdown();
	}


	void InitNetwork()
	{
		Socket::InitNetwork();
		
		m_pServer.reset(new PeerServer(m_networkThread.GetCallQueue(), boost::bind(&GameNetworkPacketTranslator::HandlePackets, &m_gameNetworkPacketTranslator, _1, _2)));
		m_pServer->Start();
		m_pServer->InitiateSession("127.0.0.1", 4242);
	}


	void ShutdownNetwork()
	{
		const bool bInvokedOnDispatchThread = false;
		m_pServer->Stop(bInvokedOnDispatchThread);
		m_pServer.reset();
		
		Socket::DeinitNetwork();
	}


	scoped_ptr<WindowsMessageInput> m_pWindowsMessageInput;
	GameNetworkPacketTranslator m_gameNetworkPacketTranslator;
	GameLoop m_gameLoop;
	DispatchThread m_networkThread;
	scoped_ptr<PeerServer> m_pServer;
	RenderWindow m_renderWindow;
	scoped_ptr<RenderWorker> m_pWorker;
};


// -----------------------------------------------------------------------------


int main(int argc, char* argv[])
{
	App app;

	app.Init();

	WPARAM exitCode = app.MessageLoop();

	app.Shutdown();

	return exitCode;
}