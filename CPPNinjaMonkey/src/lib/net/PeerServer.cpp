#include "net/PeerServer.h"
#include "PeerServerConnectionListener.h"
#include "PeerServerSession.h"

#include "framework/CallQueue.h"

#include "boost/detail/interlocked.hpp"
#include "boost/scoped_ptr.hpp"
#include "boost/bind.hpp"
#include "boost/thread/mutex.hpp"
#include "boost/thread/locks.hpp"

#include <vector>
#include <iostream>


// ----------------------------------------------------------------------------


using namespace boost;
using namespace std;


// ----------------------------------------------------------------------------


class PeerServer::Impl
{
public:
	Impl(CallQueue& callQueue, boost::function<void (SessionID, std::vector<NetworkPacket>)> packetsReceived) : 
		m_callQueue(callQueue), 
	    m_packetsReceived(packetsReceived),
		m_nSessionIDSeed(0)
	{
		m_pListener.reset(new PeerServerConnectionListener(4242, bind(&Impl::AddSession, this, _1)));
	}


	// Note: All threads have been shut down on Stop, so no synchronization is needed.
	~Impl()
	{
		m_pListener.reset();

		for(SessionList::iterator i = m_sessions.begin(); i != m_sessions.end(); ++i)
		{
			delete i->m_pSession;
		}
		m_sessions.clear();
	}


	void Start()
	{
		m_pListener->Start();
	}


	void Stop(bool bInvokedOnDispatchThread)
	{
		// 1) Stop accepting new sessions.
		m_pListener->Stop();

		// 2) Stop existing sessions from receiving.
		{
			lock_guard<mutex> lock(m_sessionsMutex);
			for(SessionList::iterator i = m_sessions.begin(); i != m_sessions.end(); ++i)
			{
				i->m_pSession->Stop();
			}
		}

		// 3) Wait until all queued calls have been dispatched.
		m_callQueue.WaitForPendingCalls(bInvokedOnDispatchThread);
	}


private:
	struct SessionInfo
	{
		SessionInfo(SessionID nSessionID, PeerServerSession *pSession) : 
			m_pSession(pSession), 
			m_nSessionID(nSessionID)
		{
		}

		PeerServerSession *m_pSession;
		SessionID m_nSessionID;
	};


	typedef vector<SessionInfo> SessionList;


	// Thread: PeerServerConnectionListener connection listener thread
	void AddSession(Socket* pSocket)
	{
		m_callQueue.Enqueue(bind(&Impl::CreateSession, this, pSocket));
	}


	// Thread: PeerServerSession receiver thread
	void OnPacketsReceived(SessionID nSessionID, vector<NetworkPacket> packets)
	{
		m_callQueue.Enqueue(bind(&Impl::HandlePackets, this, nSessionID, packets));
	}


	void HandlePackets(SessionID nSessionID, vector<NetworkPacket> packets)
	{
		for(std::vector<NetworkPacket>::const_iterator i = packets.begin(); i != packets.end(); ++i)
		{
			switch(i->Type())
			{
			case NETWORK_PACKET_TYPE_DISCONNECT:
				DeleteSession(nSessionID);
				break;
			default:
				break;
			}
		}

		// Pass packets on to application.
		m_packetsReceived(nSessionID, packets);
	}


	void CreateSession(Socket* pSocket) 
	{
		SessionID nSessionID = BOOST_INTERLOCKED_INCREMENT(&m_nSessionIDSeed);

		PeerServerSession* pSession = new PeerServerSession(pSocket, bind(&Impl::OnPacketsReceived, this, nSessionID, _1));

		{
			lock_guard<mutex> lock(m_sessionsMutex);
			m_sessions.push_back(SessionInfo(nSessionID, pSession));
		}

		// "Receive" connection pseudo-packet.
		vector<NetworkPacket> initialPackets;
		initialPackets.push_back(NetworkPacket(NETWORK_PACKET_TYPE_CONNECT, 0, NULL));
		OnPacketsReceived(nSessionID, initialPackets);

		pSession->Start();
	}


	void DeleteSession(SessionID nSessionID)
	{
		PeerServerSession* pSession = NULL;

		{
			lock_guard<mutex> lock(m_sessionsMutex);
			for(SessionList::iterator i = m_sessions.begin(); i != m_sessions.end(); ++i)
			{
				if(i->m_nSessionID == nSessionID)
				{
					pSession = i->m_pSession;
					m_sessions.erase(i);
					break;
				}
			}
		}

		if(pSession != NULL)
		{
			pSession->Stop();
			delete pSession;
		}
	}


	CallQueue& m_callQueue;
	volatile long m_nSessionIDSeed;
	mutex m_sessionsMutex;
	SessionList m_sessions;
	boost::function<void (SessionID, std::vector<NetworkPacket>)> m_packetsReceived;

	scoped_ptr<PeerServerConnectionListener> m_pListener;
};


// -----------------------------------------------------------------------------


PeerServer::PeerServer(CallQueue& callQueue, boost::function<void (SessionID, std::vector<NetworkPacket>)> packetsReceived) : m_pImpl(new Impl(callQueue, packetsReceived))
{
}


PeerServer::~PeerServer()
{
	delete m_pImpl;
}

void PeerServer::Start()
{
	m_pImpl->Start();
}

void PeerServer::Stop(bool bInvokedOnDispatchThread)
{
	m_pImpl->Stop(bInvokedOnDispatchThread);
}
