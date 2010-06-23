#pragma once
#ifndef GAMENETWORKPACKETTRANSLATOR_H_INCLUDED
#define GAMENETWORKPACKETTRANSLATOR_H_INCLUDED


#include <vector>

#include "net/PeerServer.h"
#include "GameLoop.h"


// -----------------------------------------------------------------------------


class GameNetworkPacketTranslator
{
public:
	GameNetworkPacketTranslator(GameLoop& gameLoop);
	~GameNetworkPacketTranslator();

	void HandlePackets(SessionID nSessionID, std::vector<NetworkPacket> packets);

private:
	class Impl;

	Impl* m_pImpl;
};


#endif // GAMENETWORKPACKETTRANSLATOR_H_INCLUDED