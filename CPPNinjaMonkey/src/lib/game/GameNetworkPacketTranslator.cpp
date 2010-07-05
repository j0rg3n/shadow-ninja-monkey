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


class GameNetworkPacketTranslator::Impl
{
public:
	Impl(GameLoop& gameLoop) : m_gameLoop(gameLoop)
	{
	}


	void HandlePackets(SessionID nSessionID, std::vector<NetworkPacket> packets)
	{
		for(std::vector<NetworkPacket>::const_iterator i = packets.begin(); i != packets.end(); ++i)
		{
			TRACE("Received packet from peer %d: Type: %d, Length: %d", nSessionID, i->Type(), i->Length());

			switch (i->Type())
			{
			case GAME_NETWORK_PACKET_TYPE_POSITION:
				assert(i->Length() == sizeof(PositionData));
				HandlePacket(nSessionID, (const PositionData*) i->Data());
				break;

			case NETWORK_PACKET_TYPE_CONNECT:
				m_gameLoop.PeerJoined(nSessionID);
				break;

			case NETWORK_PACKET_TYPE_DISCONNECT:
				m_gameLoop.PeerLeft(nSessionID);
				break;

			default:
				// Unknown packet.
				break;
			}
		}
	}


private:
	void HandlePacket(SessionID nSessionID, const PositionData* position) 
	{
		m_gameLoop.UpdatePosition(nSessionID,
			NetworkEndian::Swap(position->x),
			NetworkEndian::Swap(position->y));
	}


	GameLoop& m_gameLoop;
};


// -----------------------------------------------------------------------------


GameNetworkPacketTranslator::GameNetworkPacketTranslator(GameLoop& gameLoop) : m_pImpl(new Impl(gameLoop))
{
}


GameNetworkPacketTranslator::~GameNetworkPacketTranslator()
{
	delete m_pImpl;
}


void GameNetworkPacketTranslator::HandlePackets(SessionID nSessionID, std::vector<NetworkPacket> packets)
{
	m_pImpl->HandlePackets(nSessionID, packets);
}
