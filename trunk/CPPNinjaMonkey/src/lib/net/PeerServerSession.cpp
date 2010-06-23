#include "PeerServerSession.h"

#include "boost/scoped_ptr.hpp"
#include "boost/thread.hpp"

#include "NetworkPacketMarshaller.h"


// ----------------------------------------------------------------------------


using namespace boost;
using namespace std;


// ----------------------------------------------------------------------------


class PeerServerSession::Impl
{
public:
	Impl(Socket* pSocket, boost::function<void (std::vector<NetworkPacket>)> packetsReceived) : 
		m_pSocket(pSocket),
		m_packetsReceived(packetsReceived),
		m_marshaller(*pSocket, packetsReceived)
	{
	}


	~Impl()
	{
	}


	void Start()
	{
		m_receiverThread = thread(bind(&NetworkPacketMarshaller::Receive, &m_marshaller));
	}


	void Stop()
	{
		// Calling disconnect will cause the marshaller to stop.
		m_pSocket->Disconnect();
		m_receiverThread.join();
	}


	void Send(vector<NetworkPacket> packets)
	{
		m_marshaller.Send(packets);
	}


private:
	scoped_ptr<Socket> m_pSocket;

	// TODO: Replace thread with overlapped IO.
	thread m_receiverThread;
	NetworkPacketMarshaller m_marshaller;
	boost::function<void (std::vector<NetworkPacket>)> m_packetsReceived;
};


// -----------------------------------------------------------------------------


PeerServerSession::PeerServerSession(Socket* pSocket, boost::function<void (std::vector<NetworkPacket>)> packetsReceived) : 
	m_pImpl(new Impl(pSocket, packetsReceived))
{
}


PeerServerSession::~PeerServerSession()
{
	delete m_pImpl;
}


void PeerServerSession::Start()
{
	m_pImpl->Start();
}


void PeerServerSession::Stop()
{
	m_pImpl->Stop();
}


void PeerServerSession::Send(vector<NetworkPacket> packets)
{
	m_pImpl->Send(packets);
}