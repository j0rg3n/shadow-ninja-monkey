#include "App.h"

#include <iostream>

#include "input/Input.h"

#include "render/RenderWindow.h"
#include "render/RenderWorker.h"

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
	AppImpl()
	{
		m_pGameLoop.reset(GameLoop::CreateInstance());
		m_pGameNetworkPacketTranslator.reset(GameNetworkPacketTranslator::CreateInstance(*m_pGameLoop, boost::bind(&AppImpl::OnPacketsSent, this, _1, _2)));
	}


	virtual ~AppImpl()
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
		m_pGameLoop->Run();
		m_pWorker->SetEntities(m_pGameLoop->GetEntities());

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
	void OnAxisUpdate(const std::string& sAxisName, float nValue)
	{
		TRACE("%s = %.2f", sAxisName.c_str(), nValue);
	}


	void OnButtonUpdate(const std::string& sButtonName, bool bPressed)
	{
		if (bPressed)
		{
			if (sButtonName == "ESCAPE")
			{
				m_pRenderWindow->Close();
			}
		}
	}


	void OnPacketsSent(SessionID nSessionID, vector<NetworkPacket> packets)
	{
		m_pServer->Send(nSessionID, packets);
	}


	void InitInput()	
	{
		m_pInput.reset(Input::CreateInstance(*m_pRenderWindow));
		m_pInput->ConnectButtonUpdateSlot(boost::bind(&AppImpl::OnButtonUpdate, this, _1, _2));
		m_pInput->ConnectButtonUpdateSlot(boost::bind(&GameLoop::OnButtonUpdate, m_pGameLoop.get(), _1, _2));
		m_pInput->ConnectAxisUpdateSlot(boost::bind(&GameLoop::OnAxisUpdate, m_pGameLoop.get(), _1, _2));
	}


	void ShutdownInput()	
	{
		m_pInput.reset();
	}


	void InitRender()
	{
		m_pRenderWindow.reset(RenderWindow::CreateInstance());
		m_pRenderWindow->Init();

		m_pWorker.reset(RenderWorker::CreateInstance(*m_pRenderWindow));
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
		
		m_pServer.reset(PeerServer::CreateInstance(m_networkThread.GetCallQueue(), boost::bind(&GameNetworkPacketTranslator::HandleIncomingPackets, m_pGameNetworkPacketTranslator.get(), _1, _2)));
		bool bSuccess = m_pServer->Start(4250);
		assert(bSuccess); // Cannot listen on socket.
		m_pServer->InitiateSession("127.0.0.1", 4250);
	}


	void ShutdownNetwork()
	{
		const bool NOT_INVOKED_ON_DISPATCH_THREAD = false;
		m_pServer->Stop(NOT_INVOKED_ON_DISPATCH_THREAD);
		m_pServer.reset();
		
		Socket::DeinitNetwork();
	}


	scoped_ptr<Input> m_pInput;
	scoped_ptr<GameNetworkPacketTranslator> m_pGameNetworkPacketTranslator;
	scoped_ptr<GameLoop> m_pGameLoop;
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
