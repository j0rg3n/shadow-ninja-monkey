#include "PeerServer.h"
#include "PeerServerConnectionListener.h"
#include "PeerServerSession.h"

#include "boost/scoped_ptr.hpp"
#include "boost/bind.hpp"
#include "boost/thread/mutex.hpp"
#include "boost/thread/locks.hpp"

#include <vector>


// ----------------------------------------------------------------------------


using namespace boost;
using namespace std;


// ----------------------------------------------------------------------------


class PeerServer::Impl
{
public:
	Impl()
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

		m_sessions.push_back(new PeerServerSession(pSocket));
	}


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