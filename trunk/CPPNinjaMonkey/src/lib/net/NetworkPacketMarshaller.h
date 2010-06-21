#pragma once
#ifndef NETWORKPACKETMARSHALLER_H_INCLUDED
#define NETWORKPACKETMARSHALLER_H_INCLUDED


#include <vector>
#include "boost/function.hpp"
#include "NetworkPacket.h"
#include "Socket.h"


// -----------------------------------------------------------------------------


class NetworkPacketMarshaller
{
public:
	NetworkPacketMarshaller(Socket& socket, boost::function<void (std::vector<NetworkPacket>)> packetsReceived);
	~NetworkPacketMarshaller();

	void Receive();
	void Send(std::vector<NetworkPacket> packets);

private:
	class Impl;

	Impl* m_pImpl;
};


#endif // NETWORKPACKETMARSHALLER_H_INCLUDED