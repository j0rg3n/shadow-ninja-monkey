#include "lib/net/Neon.h"
#include "lib/net/Socket.h"
#include "diag/Trace.h"

#include "neon/ne_session.h"
#include "neon/ne_request.h"
#include "neon/ne_auth.h" // for authorization.
#include "neon/ne_socket.h"

#include <iostream>
#include <sstream>
#include <fstream>
#include <cassert>

#include "boost/scoped_ptr.hpp"
#include "boost/property_tree/ptree.hpp"
#include "boost/property_tree/ini_parser.hpp"


// -----------------------------------------------------------------------------


using namespace std;
using namespace boost;
using namespace boost::property_tree;


// -----------------------------------------------------------------------------


static int GetProxyAuthentication(void *pUserdata, const char *pszRealm, int nAttempt, char *pszUsername, char *pszPassword);


// -----------------------------------------------------------------------------


class HTTPSessionImpl : public HTTPSession
{
public:
	HTTPSessionImpl(const char* pszScheme, const char* pszHostName, boost::uint32_t nPort)
	{
		m_pSession = ne_session_create(pszScheme, pszHostName, nPort);

		if(ReadProxyCredentials("proxy.ini"))
		{
			// TODO: Consider using ne_session_system_proxy.

			ne_session_proxy(m_pSession, GetProxy().c_str(), 8080);
			ne_set_proxy_auth(m_pSession, &GetProxyAuthentication, this);
		}

		ne_set_notifier(m_pSession, &HTTPSessionImpl::OnSessionStatusNotify, this);
	}


	virtual ~HTTPSessionImpl()
	{
		ne_session_destroy(m_pSession);
	}


	virtual const std::string& GetBoundAddress() const { return m_sBoundAddress; }
	virtual boost::uint32_t GetBoundPort() const { return m_nBoundPort; }


	ne_session* GetNeonSession() { return m_pSession; }


	const std::string& GetProxy() const { return m_sProxy; }
	const std::string& GetUsername() const { return m_sUsername; }
	const std::string& GetPassword() const { return m_sPassword; }


private:

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


	//! \brief Gets the socket field of a session.
	//! Peeking into ne_private.h reveals that this is the first field in the structure.
	static ne_socket* GetSessionSocket(ne_session* pSession)
	{
		return *reinterpret_cast<ne_socket**>(pSession);
	}


	//! \brief Gets a Socket from a ne_socket.
	//! Peeking into ne_socket.c reveals that the socket handle is the first field in the structure.
	static int GetSocketHandle(ne_socket* pSocket)
	{
		return *reinterpret_cast<int*>(pSocket);
	}

	static void OnSessionStatusNotify(void *userdata, ne_session_status status, const ne_session_status_info *info)
	{
		HTTPSessionImpl* pThis = (HTTPSessionImpl*) userdata;
		if (status == ne_status_connected)
		{
			// Destination host: info->cd.hostname;

			// Grab bound address.
			int nSocketHandle = GetSocketHandle(GetSessionSocket(pThis->m_pSession));
			scoped_ptr<Socket> pSocket(new Socket((SOCKET) nSocketHandle));

			bool bSuccess = pSocket->GetLocalEndPoint(pThis->m_sBoundAddress, pThis->m_nBoundPort);
			assert(bSuccess);

			TRACE("Connected to web server; bound address is %s:%d.", pThis->m_sBoundAddress.c_str(), pThis->m_nBoundPort);
		}
	}


	ne_session* m_pSession;

	std::string m_sProxy;
	std::string m_sUsername;
	std::string m_sPassword;

	std::string m_sBoundAddress;
	boost::uint32_t m_nBoundPort;
};


class HTTPRequestImpl : public HTTPRequest
{
public:

	HTTPRequestImpl(HTTPSession& session, const char* pszMethod, const string& sPath) : m_session((HTTPSessionImpl&)session)
	{
		m_pRequest = ne_request_create(m_session.GetNeonSession(), pszMethod, sPath.c_str());

		ne_add_response_body_reader(m_pRequest, Accept, Read, this);
	}


	virtual ~HTTPRequestImpl()
	{
		ne_request_destroy(m_pRequest);
	}


	virtual bool Dispatch()
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
			std::cerr << "An error occurred: " << ne_get_error(m_session.GetNeonSession()) << std::endl;
			return false;
		}
	}


	virtual std::string GetResult() const
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
		HTTPRequestImpl* pThis = (HTTPRequestImpl*) pUserdata;

		pThis->m_resultStream.write(pszBuf, nLen);

		bool bSuccess = true;
		
		// "Returns zero on success, or non-zero on error. [...] The request 
		// dispatch (or ne_read_response_block call) will fail with NE_ERROR; 
		// the session error string should have been set by the callback."
		//ne_set_error(pThis->m_session, "Foo %s", "bar");
		return bSuccess ? 0 : 1;
	}


	ostringstream m_resultStream;
	HTTPSessionImpl& m_session;
	ne_request* m_pRequest;
};


// -----------------------------------------------------------------------------


HTTPSession* HTTPSession::CreateInstance(const char* pszScheme, const char* pszHostName, boost::uint32_t nPort)
{
	return new HTTPSessionImpl(pszScheme, pszHostName, nPort);
}


std::string HTTPSession::GetURLEscaped(const std::string& sText)
{
	char* pszEscapedText = ne_path_escape(sText.c_str());
	string result(pszEscapedText);
	free(pszEscapedText);

	return result;
}


void HTTPSession::WriteURLEscaped(std::ostream& out, const std::string& sText)
{
	char* pszEscapedText = ne_path_escape(sText.c_str());
	out << pszEscapedText;
	free(pszEscapedText);
}


HTTPRequest* HTTPRequest::CreateInstance(HTTPSession& session, const char* pszMethod, const char* pszPath)
{
	return new HTTPRequestImpl(session, pszMethod, pszPath);
}


// -----------------------------------------------------------------------------


static int GetProxyAuthentication(void *pUserdata, const char *pszRealm, int nAttempt, char *pszUsername, char *pszPassword)
{
	HTTPSessionImpl* pSession = (HTTPSessionImpl*) pUserdata;

	assert(pSession->GetUsername().size() + 1 <= NE_ABUFSIZ);
	strcpy(pszUsername, pSession->GetUsername().c_str());

	assert(pSession->GetPassword().size() + 1 <= NE_ABUFSIZ);
	strcpy(pszPassword, pSession->GetPassword().c_str());

	// "To retry the request using new authentication credentials, the callback 
	// should return zero. [...] To abort the request, the callback should return 
	// a non-zero value"
	return nAttempt == 0 ? 0 : 1;
}
