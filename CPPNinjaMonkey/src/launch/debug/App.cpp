#include "App.h"

#include <iostream>

#include "input/Input.h"

#include "render/RenderWindow.h"
#include "RenderWorker.h"

#include "diag/Trace.h"

#include "boost/scoped_ptr.hpp"
#include "boost/bind.hpp"
#include "boost/date_time/posix_time/posix_time.hpp"

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
using namespace boost::posix_time;


// -----------------------------------------------------------------------------


class AppImpl : public App
{
public:
	AppImpl() : m_gameNetworkPacketTranslator(m_gameLoop), m_movementVector(0, 0)
	{
		m_entities.push_back(new Entity(MakeSquare(), Point(0, 0)));
		m_entities.push_back(new Entity(MakeSquare(), Point(1, 1)));
		m_entities.push_back(new Entity(MakeSquare(), Point(2, 2)));
	}


	virtual ~AppImpl()
	{
		for (vector<Entity*>::iterator entity = m_entities.begin(); entity != m_entities.end(); ++entity)
		{
			delete *entity;
		}
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
		static ptime startTime = microsec_clock::universal_time();
		ptime now = microsec_clock::universal_time();
		
		float t = (now - startTime).total_milliseconds() / 1000.0f;

		int index = 0;
		for (vector<Entity*>::iterator entity = m_entities.begin(); entity != m_entities.end(); ++entity)
		{
			if (index == 0 || index == 1)
			{
				float angle = index * 3.14f + t * (2 * 3.14f / 4);
				float r = 2.0f;
				(*entity)->m_pos.x = cosf(angle) * r;
				(*entity)->m_pos.y = sinf(angle) * r;
			}
			else
			{
				(*entity)->m_pos.x += m_movementVector.x * t * 0.001f;
				(*entity)->m_pos.y += m_movementVector.y * t * 0.001f;
			}

			++index;
		}

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

		if (sButtonName == "RIGHT")
		{
			m_movementVector.x += bPressed ? .5f : -.5f;
		}
		else if (sButtonName == "LEFT")
		{
			m_movementVector.x -= bPressed ? .5f : -.5f;
		}		
		else if (sButtonName == "UP")
		{
			m_movementVector.y += bPressed ? .5f : -.5f;
		}		
		else if (sButtonName == "DOWN")
		{
			m_movementVector.y -= bPressed ? .5f : -.5f;
		}		

		if (bPressed)
		{
			if (sButtonName == "ESCAPE")
			{
				m_pRenderWindow->Close();
			}
		}
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
		m_pWorker.reset(RenderWorker::CreateInstance(*m_pRenderWindow));
		for (vector<Entity*>::iterator entity = m_entities.begin(); entity != m_entities.end(); ++entity)
		{
			m_pWorker->AddEntity(*entity);
		}

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


	static Geometry* MakeSquare()
	{
		Triangle t;

		Geometry* pSquare = new Geometry();

		t.m_points[0] = Point(-.5f, -.5f);
		t.m_points[1] = Point(-.5f,  .5f);
		t.m_points[2] = Point( .5f,  .5f);
		pSquare->m_triangles.push_back(t);

		t.m_points[0] = Point(-.5f, -.5f);
		t.m_points[1] = Point( .5f, -.5f);
		t.m_points[2] = Point( .5f,  .5f);
		pSquare->m_triangles.push_back(t);

		return pSquare;
	}


	Point m_movementVector;
	vector<Entity *> m_entities;
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
