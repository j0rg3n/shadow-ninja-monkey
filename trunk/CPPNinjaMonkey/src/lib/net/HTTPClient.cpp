#include "HTTPClient.h"

#ifdef _WINDOWS
#include <winsock2.h>
#include <ws2tcpip.h>
#endif

#include <cassert>
#include <sstream>

#include "Neon.h"
#include "boost/smart_ptr/scoped_ptr.hpp"


// ----------------------------------------------------------------------------


using namespace boost;
using namespace std;


// ----------------------------------------------------------------------------


class HTTPClient::Impl
{
public:
	Impl()
	{
	}

	bool Connect(string sAddress, uint32_t nPort)
	{
		assert(NULL == pSession.get());
		pSession.reset(HTTPSession::CreateInstance("http", sAddress.c_str(), nPort));
		return true;
	}

	bool Get(std::string sPath, std::string& sResult)
	{
		assert(NULL == pRequest.get());
		pRequest.reset(HTTPRequest::CreateInstance(*pSession, "GET", sPath.c_str()));
		return pRequest->Dispatch();
	}

private:
	scoped_ptr<HTTPSession> pSession;
	scoped_ptr<HTTPRequest> pRequest;
};


// ----------------------------------------------------------------------------


HTTPClient::HTTPClient() : pImpl(new Impl())
{
}


HTTPClient::~HTTPClient()
{
}


bool HTTPClient::Connect(std::string sAddress, boost::uint32_t nPort)
{
	return pImpl->Connect(sAddress, nPort);
}


void HTTPClient::Disconnect()
{
	//pImpl->Disconnect();
}


bool HTTPClient::Get(std::string sPath, std::string& sResult)
{
	return pImpl->Get(sPath, sResult);
}
