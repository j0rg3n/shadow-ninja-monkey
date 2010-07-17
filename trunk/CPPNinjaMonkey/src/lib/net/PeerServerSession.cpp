#include "PeerServerSession.h"

#include "boost/scoped_ptr.hpp"
#include "boost/thread.hpp"

#include "diag/Trace.h"

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


	void Start(std::string sAddress, boost::uint32_t nPort)
	{
		m_receiverThread = thread(boost::bind(&Impl::ConnectAndReceive, this, sAddress, nPort));
	}


	void Start()
	{
		m_receiverThread = thread(boost::bind(&Impl::Receive, this));
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
	void ConnectAndReceive(std::string sAddress, boost::uint32_t nPort)
	{
		TRACE("Connecting to %s:%d...", sAddress.c_str(), nPort);
		vector<NetworkPacket> receivedPackets;
		if(!m_pSocket->Connect(sAddress, nPort))
		{
			receivedPackets.push_back(NetworkPacket(NETWORK_PACKET_TYPE_DISCONNECT, 0, NULL));
			m_packetsReceived(receivedPackets);
			return;
		}

		TRACE("Connected to %s:%d.", sAddress.c_str(), nPort);
		Receive();
		TRACE("Disconnected from %s:%d.", sAddress.c_str(), nPort);
	}


	void Receive()
	{
		TRACE("Receiving data...");
		m_marshaller.Receive();
		TRACE("Receiving stopped.");
	}


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


void PeerServerSession::Start(std::string sAddress, boost::uint32_t nPort)
{
	m_pImpl->Start(sAddress, nPort);
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