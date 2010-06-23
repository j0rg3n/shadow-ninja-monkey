#include "NetworkPacketMarshaller.h"

#include "net/NetworkEndian.h"
#include "framework/AutoResetEvent.h"


// ----------------------------------------------------------------------------


using namespace std;
using namespace boost;


// ----------------------------------------------------------------------------


class NetworkPacketMarshaller::Impl
{
public:
	Impl(Socket& socket, boost::function<void (std::vector<NetworkPacket>)> packetsReceived) : 
	  m_socket(socket),
	  m_packetsReceived(packetsReceived),
	  m_bLoginSent(false)
	{
	}


	void Receive()
	{
		m_connectedEvent.Set();

		vector<NetworkPacket> receivedPackets;

		char aBuf[NetworkPacket::MAX_PACKET_LENGTH];

		size_t nReadCount = m_socket.Read(aBuf, 4);
		if(nReadCount == Socket::READ_ERROR)
		{
			receivedPackets.push_back(NetworkPacket(NETWORK_PACKET_TYPE_DISCONNECT, 0, NULL));
			m_packetsReceived(receivedPackets);
			return;
		}

		uint32_t nByteOrderMark = ((uint32_t*)aBuf)[0];
		// TODO: Implement byte-swapping when communicating across endians.
		assert(nByteOrderMark == NetworkPacket::BYTE_ORDER_MARK);

		char* pReceiveStart = aBuf;
		const char* pBufferEnd = aBuf + NetworkPacket::MAX_PACKET_LENGTH;
		while(true)
		{
			size_t nReadCount = m_socket.Read(pReceiveStart, pBufferEnd - pReceiveStart);
			if (nReadCount == Socket::READ_ERROR)
			{
				receivedPackets.push_back(NetworkPacket(NETWORK_PACKET_TYPE_DISCONNECT, 0, NULL));
				m_packetsReceived(receivedPackets);
				return;
			}

			const char* pPacketStart = aBuf;
			const char* pReceiveEnd = pReceiveStart + nReadCount;
			while(pPacketStart < pReceiveEnd)
			{
				size_t nReceivedBytesLeft = pReceiveEnd - pPacketStart;
				if(nReceivedBytesLeft < PACKET_HEADER_SIZE)
				{
					break;
				}

				uint32_t nDataLength = NetworkEndian::Swap(((uint32_t*)pPacketStart)[0]);
				uint32_t nPacketLength = nDataLength + PACKET_HEADER_SIZE;
				assert(nPacketLength <= NetworkPacket::MAX_PACKET_LENGTH);
				if (nReceivedBytesLeft < nPacketLength)
				{
					break;
				}

				ENetworkPacketType eType = (ENetworkPacketType) NetworkEndian::Swap(((uint32_t*)pPacketStart)[1]);
				receivedPackets.push_back(NetworkPacket(eType, nDataLength, pPacketStart + PACKET_HEADER_SIZE));

				pPacketStart += nPacketLength;
			}

			size_t nReceivedBytesLeft = pReceiveEnd - pPacketStart;
			if(nReceivedBytesLeft > 0)
			{
				// Partial packets remain. Move these to start of buffer to 
				// maximize available buffer space.
				memcpy(aBuf, pPacketStart, nReceivedBytesLeft);
			}

			// Start reception from the end of the remaining data.
			pReceiveStart = aBuf + nReceivedBytesLeft;

			// Deliver packets upstream
			m_packetsReceived(receivedPackets);
			receivedPackets.clear();
		}
	}


	bool Send(vector<NetworkPacket> packets)
	{
		if(!m_bLoginSent)
		{
			m_connectedEvent.Wait();

			m_socket.Write((const char*) &NetworkPacket::BYTE_ORDER_MARK, 4);
			m_bLoginSent = true;
		}

		for(vector<NetworkPacket>::const_iterator i = packets.begin(); i != packets.end(); ++i)
		{
			// TODO: Send in fewer calls.
			uint32_t nLength = (uint32_t) i->Length();
			if (!m_socket.Write((const char*) &nLength, 4))
			{
				return false;
			}
			
			uint32_t nType = (uint32_t) i->Type();
			if (!m_socket.Write((const char*) &nType, 4))
			{
				return false;
			}
			
			if (!m_socket.Write(i->Data(), i->Length()))
			{
				return false;
			}
		}

		return true;
	}


private:
	static const size_t PACKET_HEADER_SIZE = 8;

	// Send data
	bool m_bLoginSent;

	// Receive data
	boost::function<void (std::vector<NetworkPacket>)> m_packetsReceived;

	// Common data: Must be threadsafe.
	Socket& m_socket;

	AutoResetEvent m_connectedEvent;
};


// -----------------------------------------------------------------------------


NetworkPacketMarshaller::NetworkPacketMarshaller(Socket& socket, boost::function<void (std::vector<NetworkPacket>)> packetsReceived) : 
	m_pImpl(new Impl(socket, packetsReceived))
{
}


NetworkPacketMarshaller::~NetworkPacketMarshaller()
{
	delete m_pImpl;
}


void NetworkPacketMarshaller::Receive()
{
	m_pImpl->Receive();
}


void NetworkPacketMarshaller::Send(vector<NetworkPacket> packets)
{
	m_pImpl->Send(packets);
}