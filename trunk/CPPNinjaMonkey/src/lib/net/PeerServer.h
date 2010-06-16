#pragma once
#ifndef PEERSERVER_H_INCLUDED
#define PEERSERVER_H_INCLUDED


#include "Socket.h"


// -----------------------------------------------------------------------------


class PeerServer
{
public:
	PeerServer();
	~PeerServer();

	void AddSession(Socket* pSocket);	

private:
	class Impl;

	Impl* m_pImpl;
};


#endif // PEERSERVER_H_INCLUDED