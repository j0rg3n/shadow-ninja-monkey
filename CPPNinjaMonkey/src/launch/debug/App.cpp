#include "App.h"

#include <iostream>

#include "input/Input.h"

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

#include "render/GL.h"
#include "render/GLU.h" //< gluPerspective


// -----------------------------------------------------------------------------


using namespace std;
using namespace boost;
using namespace boost::signals2;


// -----------------------------------------------------------------------------


class AppImpl : public App
{
public:
	AppImpl() : m_gameNetworkPacketTranslator(m_gameLoop)
	{
	}


	virtual void Init()
	{
		TRACE("Starting debug launcher...");
		InitRender();
		InitNetwork();
		InitInput();
		TRACE("Debug launcher started.");
	}


	virtual void Shutdown()
	{
		TRACE("Shutting down debug launcher...");
		ShutdownInput();
		ShutdownNetwork();
		ShutdownRender();
		TRACE("Debug launcher shut down.");
	}


	virtual void OnIdle()
	{
		// TODO: Generalize with nice pool of worker threads and a barrier-style
		// gizmo for letting them all complete before the swap, and restart after 
		// the swap.

		// This requires more knowledge about the main game logic thread, so
		// postponing for now.
		m_pWorker->Run();
		m_pRenderWindow->Swap();
	}


	virtual connection ConnectClosedSlot(const RenderWindow::ClosedSignal::slot_type& slot)
	{
		return m_pRenderWindow->ConnectClosedSlot(slot);
	}


private:
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


	void OnAxisUpdate(const std::string& sAxisName, float nValue)
	{
		TRACE("%s = %.2f", sAxisName.c_str(), nValue);
	}


	void OnButtonUpdate(const std::string& sButtonName, bool bPressed)
	{
		TRACE("%s %s", sButtonName.c_str(), bPressed ? "down" : "up");
	}


	void InitInput()	
	{
		m_pInput.reset(Input::CreateInstance(*m_pRenderWindow));
		m_pInput->ConnectButtonUpdateSlot(boost::bind(&AppImpl::OnButtonUpdate, this, _1, _2));
		m_pInput->ConnectAxisUpdateSlot(boost::bind(&AppImpl::OnAxisUpdate, this, _1, _2));
	}


	void ShutdownInput()	
	{
		m_pInput.reset();
	}


	void InitRender()
	{
		m_pRenderWindow.reset(RenderWindow::CreateInstance());
		m_pRenderWindow->Init();

		m_pRenderWindow->ConnectSizeChangedSlot(boost::bind(&AppImpl::OnSizeChanged, this, _1, _2));

		// Create worker before calling OnSizeChanged, as the worker internally
		// creates an OpenGL render context for this thread.
		// TODO: Solve this encapsulation issue.
		m_pWorker.reset(new RenderWorker(*m_pRenderWindow));

		OnSizeChanged(m_pRenderWindow->Width(), m_pRenderWindow->Height());
	}


	void ShutdownRender()
	{
		m_pWorker.reset();
		m_pRenderWindow->Shutdown();
		m_pRenderWindow.reset();
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


	scoped_ptr<Input> m_pInput;
	GameNetworkPacketTranslator m_gameNetworkPacketTranslator;
	GameLoop m_gameLoop;
	DispatchThread m_networkThread;
	scoped_ptr<PeerServer> m_pServer;
	scoped_ptr<RenderWindow> m_pRenderWindow;
	scoped_ptr<RenderWorker> m_pWorker;
};


// -----------------------------------------------------------------------------


App* App::CreateInstance()
{
	return new AppImpl();
}
