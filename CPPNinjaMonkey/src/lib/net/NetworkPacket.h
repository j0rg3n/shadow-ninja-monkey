#pragma once
#ifndef NETWORKPACKET_H_INCLUDED
#define NETWORKPACKET_H_INCLUDED


#include "boost/cstdint.hpp"
#include "boost/shared_ptr.hpp"


// -----------------------------------------------------------------------------


enum ENetworkPacketType
{
	NETWORK_PACKET_TYPE_DISCONNECT = 0,
	NETWORK_PACKET_TYPE_WELCOME = 1,
};


// -----------------------------------------------------------------------------


struct NetworkPacketDisconnectData
{
};


// -----------------------------------------------------------------------------


// If memory fragmentation becomes an issue, this must be transmogrified into a 
// NetworkPacketBuffer which holds a continous block of headers and data.
class NetworkPacket
{
public:
	static const boost::uint32_t BYTE_ORDER_MARK = 0xBADC0DED;
	static const size_t MAX_PACKET_LENGTH = 32768;

	/*
	NetworkPacket(ENetworkPacketType eType, boost::uint32_t nLength) :
		m_eType(eType),
		m_nLength(nLength),
		m_pData(nLength != 0 ? new char[nLength] : NULL)
	{
	}
	*/


	NetworkPacket(ENetworkPacketType eType, boost::uint32_t nLength, const char* pData) :
		m_eType(eType),
		m_nLength(nLength),
		m_pData(nLength != 0 ? new char[nLength] : NULL)
	{
		memcpy(m_pData.get(), pData, nLength);
	}


	NetworkPacket(const NetworkPacket& other) :
		m_eType(other.m_eType),
		m_nLength(other.m_nLength),
		m_pData(other.m_pData)
	{
	}


	ENetworkPacketType Type() const { return m_eType; }
	size_t Length() const { return m_nLength; }
	const char* Data() const { return m_pData.get(); }


private:
	boost::uint32_t m_nLength;
	ENetworkPacketType m_eType;
	boost::shared_ptr<char> m_pData;
};


#endif // NETWORKPACKET_H_INCLUDED