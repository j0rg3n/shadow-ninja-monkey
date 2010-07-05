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


class PeerServer
{
public:
	//!\brief Note: Callback may be invoked on any thread.
	PeerServer(CallQueue& callQueue, boost::function<void (SessionID, std::vector<NetworkPacket>)> packetsReceived);
	~PeerServer();

	void Start();
	void Stop(bool bInvokedOnDispatchThread);

	void InitiateSession(std::string sAddress, boost::uint32_t nPort);

private:
	class Impl;

	Impl* m_pImpl;
};


#endif // PEERSERVER_H_INCLUDED