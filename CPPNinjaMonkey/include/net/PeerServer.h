#pragma once
#ifndef PEERSERVER_H_INCLUDED
#define PEERSERVER_H_INCLUDED


#include <vector>

#include "net/NetworkPacket.h"
#include "boost/cstdint.hpp"
#include "boost/function.hpp"


// -----------------------------------------------------------------------------


typedef boost::uint32_t SessionID;


class CallQueue;


// -----------------------------------------------------------------------------


struct PeerServer
{
	virtual ~PeerServer() {};

	virtual void Start() = 0;
	virtual void Stop(bool bInvokedOnDispatchThread) = 0;

	virtual void InitiateSession(std::string sAddress, boost::uint32_t nPort) = 0;

	virtual void Send(SessionID nSessionID, std::vector<NetworkPacket> packets) = 0;

	//!\brief Note: Callback may be invoked on any thread.
	static PeerServer* CreateInstance(CallQueue& callQueue, boost::function<void (SessionID, std::vector<NetworkPacket>)> packetsReceived);
};


#endif // PEERSERVER_H_INCLUDED