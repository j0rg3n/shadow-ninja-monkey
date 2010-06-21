#include "PeerServer.h"
#include "PeerServerConnectionListener.h"
#include "PeerServerSession.h"

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
	Impl() : m_nSessionIDSeed(0)
	{
		m_pListener.reset(new PeerServerConnectionListener(4242, bind(&Impl::AddSession, this, _1)));
		m_pListener->Start();
	}


	~Impl()
	{
		m_pListener->Stop();
		m_pListener.reset();

		for(PeerServerSessionList::iterator i = m_sessions.begin(); i != m_sessions.end(); ++i)
		{
			(*i)->Stop();
			delete *i;
		}

		m_sessions.clear();
	}


private:
	typedef vector<PeerServerSession*> PeerServerSessionList;


	// Note: Will arrive from the connection listener thread.
	void AddSession(Socket* pSocket)
	{
		lock_guard<mutex> lock(m_sessionsMutex);

		uint32_t nSessionID = m_nSessionIDSeed;
		++m_nSessionIDSeed;

		PeerServerSession* pSession = new PeerServerSession(pSocket, bind(&Impl::OnPacketsReceived, this, nSessionID, _1));
		m_sessions.push_back(pSession);
		
		cout << "Starting incoming session " << nSessionID << "...";
		pSession->Start();

		vector<NetworkPacket> initPackets;
		initPackets.push_back(NetworkPacket(NETWORK_PACKET_TYPE_WELCOME, 0, NULL));
		pSession->Send(initPackets);
	}


	void OnPacketsReceived(size_t nSessionID, vector<NetworkPacket> packets)
	{
		for(vector<NetworkPacket>::const_iterator i = packets.begin(); i != packets.end(); ++i)
		{
			cout << "Received packet: Session: " << nSessionID << ", Type: " << i->Type() << ", Length: " << i->Length() << endl;
		}
	}


	uint32_t m_nSessionIDSeed;
	mutex m_sessionsMutex;
	PeerServerSessionList m_sessions;

	scoped_ptr<PeerServerConnectionListener> m_pListener;
};


// -----------------------------------------------------------------------------


PeerServer::PeerServer() : m_pImpl(new Impl())
{
}


PeerServer::~PeerServer()
{
	delete m_pImpl;
}


void PeerServer::AddSession(Socket* pSocket)
{
}