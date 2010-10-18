#include "lib/net/Neon.h"

#include "neon/ne_session.h"
#include "neon/ne_request.h"
#include "neon/ne_auth.h" // for authorization.

#include <iostream>
#include <sstream>
#include <fstream>
#include <cassert>

#include "boost/property_tree/ptree.hpp"
#include "boost/property_tree/ini_parser.hpp"


// -----------------------------------------------------------------------------


using namespace std;
using namespace boost;
using namespace boost::property_tree;


// -----------------------------------------------------------------------------


static int GetProxyAuthentication(void *pUserdata, const char *pszRealm, int nAttempt, char *pszUsername, char *pszPassword);


// -----------------------------------------------------------------------------


class HTTPSession::Impl
{
public:
	Impl(const char* pszScheme, const char* pszHostName, boost::uint32_t nPort)
	{
		m_pSession = ne_session_create(pszScheme, pszHostName, nPort);

		if(ReadProxyCredentials("proxy.ini"))
		{
			// TODO: Consider using ne_session_system_proxy.

			ne_session_proxy(m_pSession, "eurprx01", 8080);
			ne_set_proxy_auth(m_pSession, &GetProxyAuthentication, this);
		}
	}


	~Impl()
	{
		ne_session_destroy(m_pSession);
	}


	void* GetInternalSession() { return m_pSession; }


	bool ReadProxyCredentials(std::string sFilename)
	{
		ifstream inp;
		
		inp.open(sFilename.c_str(), ifstream::in);
		if(inp.fail())
		{
			inp.clear(ios::failbit);
			return false;
		}

		ptree contents;
		try
		{
			ini_parser::read_ini(inp, contents);
			inp.close();
		}
		catch(const ini_parser::ini_parser_error& e)
		{
			inp.close();
			cerr << "ERROR: Unable to read ini file: " << e.what() << endl;
			return false;
		}

		m_sProxy = contents.get("proxy", string());
		m_sUsername = contents.get("username", string());
		m_sPassword = contents.get("password", string());
		return true;
	}


	static std::string GetURLEscaped(const std::string& sText)
	{
		char* pszEscapedText = ne_path_escape(sText.c_str());
		string result(pszEscapedText);
		free(pszEscapedText);

		return result;
	}


	static void WriteURLEscaped(std::ostream& out, const std::string& sText)
	{
		char* pszEscapedText = ne_path_escape(sText.c_str());
		out << pszEscapedText;
		free(pszEscapedText);
	}


	const std::string& GetProxy() const { return m_sProxy; }
	const std::string& GetUsername() const { return m_sUsername; }
	const std::string& GetPassword() const { return m_sPassword; }


private:
	ne_session* m_pSession;

	std::string m_sProxy;
	std::string m_sUsername;
	std::string m_sPassword;
};


class HTTPRequest::Impl
{
public:
	Impl(HTTPSession& session, const char* pszMethod, const string& sPath) : m_session(session)
	{
		m_pRequest = ne_request_create(GetNeonSession(), pszMethod, sPath.c_str());

		ne_add_response_body_reader(m_pRequest, Accept, Read, this);
	}


	~Impl()
	{
		ne_request_destroy(m_pRequest);
	}


	bool Dispatch()
	{
		switch (ne_request_dispatch(m_pRequest))
		{
		case NE_OK:
			return true;
		default:
			/*
			NE_ERROR
			Request failed (see session error string)

			NE_LOOKUP
			The DNS lookup for the server (or proxy server) failed.

			NE_AUTH
			Authentication failed on the server.

			NE_PROXYAUTH
			Authentication failed on the proxy server.

			NE_CONNECT
			A connection to the server could not be established.

			NE_TIMEOUT
			A timeout occurred while waiting for the server to respond.
			*/
			std::cerr << "An error occurred: " << ne_get_error(GetNeonSession()) << std::endl;
			return false;
		}
	}


	std::string GetResult() const
	{
		return m_resultStream.str();
	}


private:
	static int Accept(void* pUserdata, ne_request* pRequest, const ne_status* pStatus)
	{
		HTTPRequest* pThis = (HTTPRequest*) pUserdata;

		bool bIgnore;
		switch(pStatus->klass)
		{
		case 2: // Success: the operation was successful
			bIgnore = false;
			break;

		case 1: // Informational response.
		case 3: // Redirection
		case 4: // Client error: the request made was incorrect in some manner.
		case 5: // Server error
			bIgnore = true;
			break;

		default:
			assert(false);
			break;
		}

		// "Return non-zero to accept the response, else zero to ignore it."
		return bIgnore ? 0 : 1;
	}


	static int Read(void* pUserdata, const char* pszBuf, size_t nLen)
	{
		Impl* pThis = (Impl*) pUserdata;

		pThis->m_resultStream.write(pszBuf, nLen);

		bool bSuccess = true;
		
		// "Returns zero on success, or non-zero on error. [...] The request 
		// dispatch (or ne_read_response_block call) will fail with NE_ERROR; 
		// the session error string should have been set by the callback."
		//ne_set_error(pThis->m_session, "Foo %s", "bar");
		return bSuccess ? 0 : 1;
	}


	ne_session* GetNeonSession() { return (ne_session*) m_session.GetInternalSession(); }


	ostringstream m_resultStream;
	HTTPSession& m_session;
	ne_request* m_pRequest;
};


// -----------------------------------------------------------------------------


HTTPSession::HTTPSession(const char* pszScheme, const char* pszHostName, boost::uint32_t nPort) : 
  pImpl(new Impl(pszScheme, pszHostName, nPort))
{
}

	
HTTPSession::~HTTPSession()
{
	delete pImpl;
}


void* HTTPSession::GetInternalSession()
{
	return pImpl->GetInternalSession();
}


std::string HTTPSession::GetURLEscaped(const std::string& sText)
{
	return Impl::GetURLEscaped(sText);
}


void HTTPSession::WriteURLEscaped(std::ostream& out, const std::string& sText)
{
	Impl::WriteURLEscaped(out, sText);
}


HTTPRequest::HTTPRequest(HTTPSession& session, const char* pszMethod, const char* pszPath) : 
  pImpl(new Impl(session, pszMethod, pszPath))
{
}


HTTPRequest::~HTTPRequest()
{
	delete pImpl;
}


bool HTTPRequest::Dispatch()
{
	return pImpl->Dispatch();
}


std::string HTTPRequest::GetResult() const
{
	return pImpl->GetResult();
}


// -----------------------------------------------------------------------------


static int GetProxyAuthentication(void *pUserdata, const char *pszRealm, int nAttempt, char *pszUsername, char *pszPassword)
{
	HTTPSession::Impl* pSession = (HTTPSession::Impl*) pUserdata;

	assert(pSession->GetUsername().size() + 1 <= NE_ABUFSIZ);
	strcpy(pszUsername, pSession->GetUsername().c_str());

	assert(pSession->GetPassword().size() + 1 <= NE_ABUFSIZ);
	strcpy(pszPassword, pSession->GetPassword().c_str());

	// "To retry the request using new authentication credentials, the callback 
	// should return zero. [...] To abort the request, the callback should return 
	// a non-zero value"
	return nAttempt == 0 ? 0 : 1;
}
