#pragma once
#ifndef PEERCLIENT_H_INCLUDED
#define PEERCLIENT_H_INCLUDED


#include <string>
#include <vector>

#include "boost/cstdint.hpp"
#include "boost/function.hpp"

#include "net/NetworkPacket.h"


// -----------------------------------------------------------------------------


class PeerClient
{
public:
	PeerClient(boost::function<void (std::vector<NetworkPacket>)> packetsReceived);
	~PeerClient();

	void Connect(std::string sAddress, boost::uint32_t nPort);
	void Disconnect();

	void Send(std::vector<NetworkPacket> packets);

private:
	class Impl;

	Impl* m_pImpl;
};


#endif // PEERCLIENT_H_INCLUDED