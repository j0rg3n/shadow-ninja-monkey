#pragma once
#ifndef GAMENETWORKPACKETTRANSLATOR_H_INCLUDED
#define GAMENETWORKPACKETTRANSLATOR_H_INCLUDED


#include <vector>

#include "net/PeerServer.h"
#include "GameLoop.h"


// -----------------------------------------------------------------------------


struct GameNetworkPacketTranslator
{
public:
	virtual ~GameNetworkPacketTranslator() {};

	virtual void HandlePackets(SessionID nSessionID, std::vector<NetworkPacket> packets) = 0;

	static GameNetworkPacketTranslator* CreateInstance(GameLoop& gameLoop);
};


#endif // GAMENETWORKPACKETTRANSLATOR_H_INCLUDED