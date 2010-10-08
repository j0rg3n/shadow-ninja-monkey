#pragma once
#ifndef GAMELOOP_H_INCLUDED
#define GAMELOOP_H_INCLUDED

#include <vector>

#include "boost/cstdint.hpp"
#include "boost/signals2/signal.hpp"
#include "net/PeerServer.h"


// -----------------------------------------------------------------------------


class Entity;


// -----------------------------------------------------------------------------


struct GameLoop
{
	typedef boost::signals2::signal<void (boost::int32_t x, boost::int32_t y)> PlayerPositionUpdatedSignal;

	virtual ~GameLoop() {};

	virtual void Run() = 0;

	virtual std::vector<Entity*>& GetEntities() = 0;

	virtual void OnPeerJoined(SessionID nSessionID) = 0;
	virtual void OnPeerLeft(SessionID nSessionID) = 0;
	virtual void OnPeerUpdatePosition(SessionID nSessionID, boost::int32_t x, boost::int32_t y) = 0;

	virtual void OnButtonUpdate(const std::string& sButtonName, bool bPressed) = 0;
	virtual void OnAxisUpdate(const std::string& sAxisName, float nValue) = 0;

	virtual boost::signals2::connection ConnectPlayerPositionUpdatedSlot(const PlayerPositionUpdatedSignal::slot_type& slot) = 0;

	static GameLoop* CreateInstance();
};


#endif // GAMELOOP_H_INCLUDED