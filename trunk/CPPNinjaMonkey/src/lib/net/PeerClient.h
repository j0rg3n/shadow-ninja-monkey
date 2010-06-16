#pragma once
#ifndef PEERCLIENT_H_INCLUDED
#define PEERCLIENT_H_INCLUDED


// -----------------------------------------------------------------------------


class PeerClient
{
public:
	PeerClient();
	~PeerClient();

	

private:
	class Impl;

	Impl* m_pImpl;
};


#endif // PEERCLIENT_H_INCLUDED