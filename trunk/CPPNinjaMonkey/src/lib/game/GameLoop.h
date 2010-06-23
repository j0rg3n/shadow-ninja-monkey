#pragma once
#ifndef GAMELOOP_H_INCLUDED
#define GAMELOOP_H_INCLUDED


#include "boost/cstdint.hpp"
#include "net/PeerServer.h"


// -----------------------------------------------------------------------------


class GameLoop
{
public:
	GameLoop();
	~GameLoop();

	void Run();

	void PeerJoined(SessionID nSessionID);
	void PeerLeft(SessionID nSessionID);
	void UpdatePosition(SessionID nSessionID, boost::int32_t x, boost::int32_t y);

private:
	class Impl;

	Impl* m_pImpl;
};


#endif // GAMELOOP_H_INCLUDED