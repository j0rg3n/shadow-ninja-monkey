#include "PeerServerConnectionListener.h"
#include "net/PeerServer.h"
#include "Socket.h"

#include "boost/thread.hpp"


// ----------------------------------------------------------------------------


using namespace std;
using namespace boost;


// ----------------------------------------------------------------------------


class PeerServerConnectionListener::Impl
{
public:
	Impl(uint32_t nPort, boost::function<void (Socket*)> connectionAccepted) : 
	  m_nPort(nPort), 
	  m_connectionAccepted(connectionAccepted),
	  m_bQuit(false)
	{
	}


	bool Start()
	{
		assert(!m_listenerSocket.IsConnected());

		bool bResult = m_listenerSocket.Listen(m_nPort);
		if(!bResult)
		{
			return false;
		}

		m_listenerThread = thread(boost::bind(&Impl::AcceptIncomingConnections, this));
		return true;
	}


	void Stop()
	{
		if(!m_listenerSocket.IsConnected())
		{
			return;
		}

		m_bQuit = true;

		// Calling disconnect will cause Accept to stop blocking and return false.
		m_listenerSocket.Disconnect();
		m_listenerThread.join();
	}


private:
	void AcceptIncomingConnections()
	{
		while(!m_bQuit)
		{
			Socket* pConnection;
			if(m_listenerSocket.Accept(&pConnection))
			{
				m_connectionAccepted(pConnection);
			}
		}
	}


	uint32_t m_nPort;
	Socket m_listenerSocket;

	// TODO: Replace thread with overlapped IO.
	thread m_listenerThread;
	volatile bool m_bQuit;
	boost::function<void (Socket*)> m_connectionAccepted;
};


// -----------------------------------------------------------------------------


PeerServerConnectionListener::PeerServerConnectionListener(boost::uint32_t nPort, boost::function<void (Socket*)> connectionAccepted) : 
	m_pImpl(new Impl(nPort, connectionAccepted))
{
}


PeerServerConnectionListener::~PeerServerConnectionListener()
{
	delete m_pImpl;
}


bool PeerServerConnectionListener::Start()
{
	return m_pImpl->Start();
}


void PeerServerConnectionListener::Stop()
{
	m_pImpl->Stop();
}
