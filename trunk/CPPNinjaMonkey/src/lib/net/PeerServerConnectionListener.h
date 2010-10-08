#pragma once
#ifndef PEERSERVERCONNECTIONLISTENER_H_INCLUDED
#define PEERSERVERCONNECTIONLISTENER_H_INCLUDED


#include "boost/cstdint.hpp"
#include "boost/function.hpp"
#include "Socket.h"


// -----------------------------------------------------------------------------


class PeerServerConnectionListener
{
public:
	//!\brief Warning: The connectionAccepted callback will be performed on an arbitrary thread.
	PeerServerConnectionListener(boost::function<void (Socket*)> connectionAccepted);
	~PeerServerConnectionListener();

	bool Start(boost::uint32_t nPort);
	void Stop();

private:
	class Impl;

	Impl* m_pImpl;
};


#endif // PEERSERVERCONNECTIONLISTENER_H_INCLUDED