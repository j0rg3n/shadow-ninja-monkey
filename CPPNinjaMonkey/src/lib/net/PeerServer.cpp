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


// ----------------------------------------------------------------------------


// Thread-safe list of sessions
class SessionManager
{
public:
	PeerServerSession* Find(SessionID nSessionID)
	{
		lock_guard<mutex> lock(m_sessionsMutex);
		SessionList::iterator i = FindInternal(nSessionID);
		return i != m_sessions.end() ? i->m_pSession : NULL;
	}


	void Add(SessionID nSessionID, PeerServerSession* pSession)
	{
		lock_guard<mutex> lock(m_sessionsMutex);
		m_sessions.push_back(SessionInfo(nSessionID, pSession));
	}


	PeerServerSession* Remove(SessionID nSessionID)
	{
		lock_guard<mutex> lock(m_sessionsMutex);
		SessionList::iterator i = FindInternal(nSessionID);
		if (i == m_sessions.end())
		{
			return NULL;
		}

		PeerServerSession* pSession = i->m_pSession;
		m_sessions.erase(i);
		return pSession;
	}


	void StopAll()
	{
		lock_guard<mutex> lock(m_sessionsMutex);
		for(SessionList::iterator i = m_sessions.begin(); i != m_sessions.end(); ++i)
		{
			i->m_pSession->Stop();
		}
	}


	// Warning: Deletes all sessions. Must only be run when sessions are no 
	// longer active.
	void DeleteAll()
	{
		for(SessionList::iterator i = m_sessions.begin(); i != m_sessions.end(); ++i)
		{
			delete i->m_pSession;
		}
		m_sessions.clear();
	}


private:
	typedef vector<SessionInfo> SessionList;


	SessionList::iterator FindInternal(SessionID nSessionID)
	{
		for(SessionList::iterator i = m_sessions.begin(); i != m_sessions.end(); ++i)
		{
			if(i->m_nSessionID == nSessionID)
			{
				return i;
			}
		}

		return m_sessions.end();
	}


	SessionList m_sessions;
	mutex m_sessionsMutex;
};


// ----------------------------------------------------------------------------


class PeerServerImpl : public PeerServer
{
public:
	PeerServerImpl(CallQueue& callQueue, boost::function<void (SessionID, std::vector<NetworkPacket>)> packetsReceived) : 
		m_callQueue(callQueue), 
	    m_packetsReceived(packetsReceived),
		m_nSessionIDSeed(0)
	{
		m_pListener.reset(new PeerServerConnectionListener(4242, boost::bind(&PeerServerImpl::AddSession, this, _1)));
	}


	// Note: All threads have been shut down on Stop, so no synchronization is needed.
	~PeerServerImpl()
	{
		m_pListener.reset();
		m_sessions.DeleteAll();
	}


	// Thread: Any.
	virtual void Start()
	{
		m_pListener->Start();
	}


	// Thread: Any.
	virtual void Stop(bool bInvokedOnDispatchThread)
	{
		// 1) Stop accepting new sessions.
		m_pListener->Stop();

		// 2) Stop existing sessions from receiving.
		m_sessions.StopAll();

		// 3) Wait until all queued calls have been dispatched.
		m_callQueue.WaitForPendingCalls(bInvokedOnDispatchThread);
	}


	// Thread: Any.
	virtual void InitiateSession(std::string sAddress, boost::uint32_t nPort)
	{
		m_callQueue.Enqueue(boost::bind(&PeerServerImpl::CreateAndStartSession, this, sAddress, nPort));
	}


	// Thread: Any.
	virtual void Send(SessionID nSessionID, vector<NetworkPacket> packets)
	{
		m_callQueue.Enqueue(boost::bind(&PeerServerImpl::HandleOutgoingPackets, this, nSessionID, packets));
	}


private:
	// Thread: PeerServerConnectionListener connection listener thread
	void AddSession(Socket* pSocket)
	{
		m_callQueue.Enqueue(boost::bind(&PeerServerImpl::CreateAndStartSession, this, pSocket));
	}


	// Thread: PeerServerSession receiver thread
	void OnPacketsReceived(SessionID nSessionID, vector<NetworkPacket> packets)
	{
		m_callQueue.Enqueue(boost::bind(&PeerServerImpl::HandleIncomingPackets, this, nSessionID, packets));
	}


	void HandleOutgoingPackets(SessionID nSessionID, vector<NetworkPacket> packets)
	{
		// Note: This is safe because HandleOutgoingPackets and StopAndDeleteSession runs
		// on the same thread.
		PeerServerSession* pSession = m_sessions.Find(nSessionID);
		if(pSession != NULL)
		{
			pSession->Send(packets);
		}
	}


	void HandleIncomingPackets(SessionID nSessionID, vector<NetworkPacket> packets)
	{
		for(std::vector<NetworkPacket>::const_iterator i = packets.begin(); i != packets.end(); ++i)
		{
			switch(i->Type())
			{
			case NETWORK_PACKET_TYPE_DISCONNECT:
				StopAndDeleteSession(nSessionID);
				break;
			default:
				break;
			}
		}

		// Pass packets on to application.
		m_packetsReceived(nSessionID, packets);
	}


	void CreateAndStartSession(Socket* pSocket) 
	{
		PeerServerSession* pSession = CreateSession(pSocket);
		pSession->Start();
	}


	void CreateAndStartSession(string sAddress, uint32_t nPort) 
	{
		Socket* pSocket = new Socket();
		PeerServerSession* pSession = CreateSession(pSocket);
		pSession->Start(sAddress, nPort);
	}


	PeerServerSession* CreateSession(Socket* pSocket) 
	{
		SessionID nSessionID = BOOST_INTERLOCKED_INCREMENT(&m_nSessionIDSeed);

		PeerServerSession* pSession = new PeerServerSession(pSocket, boost::bind(&PeerServerImpl::OnPacketsReceived, this, nSessionID, _1));

		m_sessions.Add(nSessionID, pSession);

		// "Receive" connection pseudo-packet.
		vector<NetworkPacket> initialPackets;
		initialPackets.push_back(NetworkPacket(NETWORK_PACKET_TYPE_CONNECT, 0, NULL));
		OnPacketsReceived(nSessionID, initialPackets);

		return pSession;
	}


	void StopAndDeleteSession(SessionID nSessionID)
	{
		PeerServerSession* pSession = m_sessions.Remove(nSessionID);
		if(pSession != NULL)
		{
			pSession->Stop();
			delete pSession;
		}
	}


	CallQueue& m_callQueue;
	volatile long m_nSessionIDSeed;
	SessionManager m_sessions;
	boost::function<void (SessionID, std::vector<NetworkPacket>)> m_packetsReceived;

	scoped_ptr<PeerServerConnectionListener> m_pListener;
};


// -----------------------------------------------------------------------------


PeerServer* PeerServer::CreateInstance(CallQueue& callQueue, boost::function<void (SessionID, std::vector<NetworkPacket>)> packetsReceived)
{
	return new PeerServerImpl(callQueue, packetsReceived);
}
