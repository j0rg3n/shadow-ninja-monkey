#pragma once
#ifndef PEERSERVERSESSION_H_INCLUDED
#define PEERSERVERSESSION_H_INCLUDED


#include "Socket.h"


// -----------------------------------------------------------------------------


class PeerServerSession
{
public:
	PeerServerSession(Socket* pSocket);
	~PeerServerSession();

	

private:
	class Impl;

	Impl* m_pImpl;
};


#endif // PEERSERVERSESSION_H_INCLUDED