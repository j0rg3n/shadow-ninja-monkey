#pragma once
#ifndef GAMENETWORKPACKETTRANSLATOR_H_INCLUDED
#define GAMENETWORKPACKETTRANSLATOR_H_INCLUDED


#include <vector>

#include "net/PeerServer.h"
#include "GameLoop.h"


// -----------------------------------------------------------------------------


struct GameNetworkPacketTranslator
{
	virtual ~GameNetworkPacketTranslator() {};

	virtual void HandleIncomingPackets(SessionID nSessionID, std::vector<NetworkPacket> packets) = 0;

	static GameNetworkPacketTranslator* CreateInstance(GameLoop& gameLoop, boost::function<void (SessionID, std::vector<NetworkPacket>)> packetsSent);
};


#endif // GAMENETWORKPACKETTRANSLATOR_H_INCLUDED