#pragma once
#ifndef PEERSERVERSESSION_H_INCLUDED
#define PEERSERVERSESSION_H_INCLUDED


#include <vector>

#include "boost/function.hpp"

#include "Socket.h"
#include "net/NetworkPacket.h"

// -----------------------------------------------------------------------------


class PeerServerSession
{
public:
	PeerServerSession(Socket* pSocket, boost::function<void (std::vector<NetworkPacket>)> packetsReceived);
	~PeerServerSession();

	void Start(std::string sAddress, boost::uint32_t nPort);
	void Start();
	void Stop();

	void Send(std::vector<NetworkPacket> packets);

private:
	class Impl;

	Impl* m_pImpl;
};


#endif // PEERSERVERSESSION_H_INCLUDED