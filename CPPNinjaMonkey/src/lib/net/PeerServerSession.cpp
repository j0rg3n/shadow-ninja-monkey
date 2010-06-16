#include "PeerServerSession.h"

#include "boost/scoped_ptr.hpp"


// ----------------------------------------------------------------------------


using namespace boost;


// ----------------------------------------------------------------------------


class PeerServerSession::Impl
{
public:
	Impl(Socket* pSocket) : m_pSocket(pSocket)
	{
	}


	~Impl()
	{
	}


private:
	scoped_ptr<Socket> m_pSocket;
};


// -----------------------------------------------------------------------------


PeerServerSession::PeerServerSession(Socket* pSocket) : m_pImpl(new Impl(pSocket))
{
}


PeerServerSession::~PeerServerSession()
{
	delete m_pImpl;
}

