#include "GameNetworkPacketTranslator.h"

#include "boost/cstdint.hpp"
#include "net/NetworkEndian.h"
#include "diag/Trace.h"


// ----------------------------------------------------------------------------


using namespace std;
using namespace boost;


// ----------------------------------------------------------------------------


enum EGameNetworkPacketType
{
	GAME_NETWORK_PACKET_TYPE_POSITION = NETWORK_PACKET_TYPE_USER + 0,
};


struct PositionData
{
	int32_t x;
	int32_t y;
};


// ----------------------------------------------------------------------------


class GameNetworkPacketTranslatorImpl : public GameNetworkPacketTranslator
{
public:
	GameNetworkPacketTranslatorImpl(GameLoop& gameLoop, boost::function<void (SessionID, std::vector<NetworkPacket>)> packetsSent) : 
		m_gameLoop(gameLoop), 
		m_packetsSent(packetsSent)
	{
		gameLoop.ConnectPlayerPositionUpdatedSlot(boost::bind(&GameNetworkPacketTranslatorImpl::OnPlayerPositionUpdated, this, _1, _2));
	}


	void HandleIncomingPackets(SessionID nSessionID, std::vector<NetworkPacket> packets)
	{
		for(std::vector<NetworkPacket>::const_iterator i = packets.begin(); i != packets.end(); ++i)
		{
			TRACE("Received packet from peer %d: Type: %d, Length: %d", nSessionID, i->Type(), i->Length());

			switch (i->Type())
			{
			case GAME_NETWORK_PACKET_TYPE_POSITION:
				assert(i->Length() == sizeof(PositionData));
				HandleIncomingPacket(nSessionID, (const PositionData*) i->Data());
				break;

			case NETWORK_PACKET_TYPE_CONNECT:
				m_gameLoop.OnPeerJoined(nSessionID);
				break;

			case NETWORK_PACKET_TYPE_DISCONNECT:
				m_gameLoop.OnPeerLeft(nSessionID);
				break;

			default:
				// Unknown packet.
				break;
			}
		}
	}


private:
	void HandleIncomingPacket(SessionID nSessionID, const PositionData* position) 
	{
		m_gameLoop.OnPeerUpdatePosition(nSessionID,
			NetworkEndian::Swap(position->x),
			NetworkEndian::Swap(position->y));
	}


	void GameNetworkPacketTranslatorImpl::OnPlayerPositionUpdated(int32_t x, int32_t y)
	{
		PositionData data;
		data.x = x;
		data.y = y;
	
		// This is awkwardly located, as the gameloop should select who to send to, and this class should only
		// create network packets. Suggested shape: Calls that append packets to an internal buffer, which can
		// then be extracted and sent to the right party.
		vector<NetworkPacket> packets;
		packets.push_back(NetworkPacket((ENetworkPacketType) GAME_NETWORK_PACKET_TYPE_POSITION, sizeof(data), (const char*) &data));
		m_packetsSent(PeerServer::BROADCAST_SESSION_ID, packets);
	}


	boost::function<void (SessionID, std::vector<NetworkPacket>)> m_packetsSent;
	GameLoop& m_gameLoop;
};


// -----------------------------------------------------------------------------


GameNetworkPacketTranslator* GameNetworkPacketTranslator::CreateInstance(GameLoop& gameLoop, boost::function<void (SessionID, std::vector<NetworkPacket>)> packetsSent)
{
	return new GameNetworkPacketTranslatorImpl(gameLoop, packetsSent);
}
