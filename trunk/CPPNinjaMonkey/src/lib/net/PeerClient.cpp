#include "PeerClient.h"


// ----------------------------------------------------------------------------


class PeerClient::Impl
{
public:
	
private:
	
};


// -----------------------------------------------------------------------------


PeerClient::PeerClient() : m_pImpl(new Impl())
{
}


PeerClient::~PeerClient()
{
	delete m_pImpl;
}

