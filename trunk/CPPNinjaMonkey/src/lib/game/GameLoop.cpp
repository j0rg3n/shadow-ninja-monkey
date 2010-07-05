#include "GameLoop.h"

#include "diag/Trace.h"


// ----------------------------------------------------------------------------


class GameLoop::Impl
{
public:
	void PeerJoined(SessionID nSessionID)
	{
		TRACE("Peer %d joined.", nSessionID);
	}


	void PeerLeft(SessionID nSessionID)
	{
		TRACE("Peer %d left.", nSessionID);
	}


	void UpdatePosition(SessionID nSessionID, boost::int32_t x, boost::int32_t y)
	{
		TRACE("Peer %d position: (%d, %d)", nSessionID, x, y);
	}	


private:
};


// -----------------------------------------------------------------------------


GameLoop::GameLoop() : m_pImpl(new Impl())
{
}


GameLoop::~GameLoop()
{
	delete m_pImpl;
}


void GameLoop::PeerJoined(SessionID nSessionID)
{
	m_pImpl->PeerJoined(nSessionID);
}


void GameLoop::PeerLeft(SessionID nSessionID)
{
	m_pImpl->PeerLeft(nSessionID);
}


void GameLoop::UpdatePosition(SessionID nSessionID, boost::int32_t x, boost::int32_t y)
{
	m_pImpl->UpdatePosition(nSessionID, x, y);
}