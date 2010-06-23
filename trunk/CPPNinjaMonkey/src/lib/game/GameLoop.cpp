#include "GameLoop.h"


// ----------------------------------------------------------------------------


class GameLoop::Impl
{
public:
	
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

