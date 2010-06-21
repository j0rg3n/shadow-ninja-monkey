#include "PeerClient.h"
#include "Socket.h"

#include <iostream>
#include "boost/thread.hpp"
#include "NetworkPacketMarshaller.h"


// ----------------------------------------------------------------------------


using namespace std;
using namespace boost;


// ----------------------------------------------------------------------------


class PeerClient::Impl
{
public:
	Impl(boost::function<void (std::vector<NetworkPacket>)> packetsReceived) :
	  m_packetsReceived(packetsReceived),
	  m_marshaller(m_socket, packetsReceived)
	{
	}


	void Connect(std::string sAddress, boost::uint32_t nPort)
	{
		m_receiverThread = thread(bind(&Impl::Receive, this, sAddress, nPort));
	}


	void Disconnect()
	{
		// Calling disconnect will cause the marshaller to stop.
		m_socket.Disconnect();
		m_receiverThread.join();
	}


	void Send(vector<NetworkPacket> packets)
	{
		m_marshaller.Send(packets);
	}

	
private:
	void Receive(std::string sAddress, boost::uint32_t nPort)
	{
		vector<NetworkPacket> receivedPackets;
		if(!m_socket.Connect(sAddress, nPort))
		{
			receivedPackets.push_back(NetworkPacket(NETWORK_PACKET_TYPE_DISCONNECT, 0, NULL));
			m_packetsReceived(receivedPackets);
			return;
		}

		m_marshaller.Receive();
	}


	Socket m_socket;
	thread m_receiverThread;
	NetworkPacketMarshaller m_marshaller;
	boost::function<void (std::vector<NetworkPacket>)> m_packetsReceived;
};


// -----------------------------------------------------------------------------


PeerClient::PeerClient(boost::function<void (std::vector<NetworkPacket>)> packetsReceived) : m_pImpl(new Impl(packetsReceived))
{
}


PeerClient::~PeerClient()
{
	delete m_pImpl;
}


void PeerClient::Connect(std::string sAddress, boost::uint32_t nPort)
{
	return m_pImpl->Connect(sAddress, nPort);
}


void PeerClient::Disconnect()
{
	m_pImpl->Disconnect();
}


void PeerClient::Send(vector<NetworkPacket> packets)
{
	m_pImpl->Send(packets);
}
