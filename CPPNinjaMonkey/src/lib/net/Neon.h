#pragma once
#ifndef NEON_H
#define NEON_H

#include "neon/ne_session.h"
#include "neon/ne_request.h"


#include <iostream>
#include <sstream>


// -----------------------------------------------------------------------------


class HTTPSession
{
public:
	HTTPSession(const char* pszScheme, const char* pszHostName, boost::uint32_t nPort)
	{
		m_pSession = ne_session_create(pszScheme, pszHostName, nPort);
	}


	~HTTPSession()
	{
		ne_session_destroy(m_pSession);
	}


	ne_session* GetInternalSession() { return m_pSession; }


private:
	ne_session* m_pSession;
};


class HTTPRequest
{
public:
	HTTPRequest(HTTPSession& session, const char* pszMethod, const char* pszPath) : m_session(session)
	{
		char* pszEscapedPath = ne_path_escape(pszPath);
		m_pRequest = ne_request_create(session.GetInternalSession(), pszMethod, pszEscapedPath);
		free(pszEscapedPath);

		ne_add_response_body_reader(m_pRequest, Accept, Read, this);
	}


	~HTTPRequest()
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
			std::cerr << "An error occurred: " << ne_get_error(m_session.GetInternalSession()) << std::endl;
			return false;
		}
	}


	std::string GetResult()
	{
		return m_resultStream.str();
	}


private:
	static int Accept(void* pUserdata, ne_request* pRequest, const ne_status* pStatus)
	{
		HTTPRequest* pThis = (HTTPRequest*) pUserdata;

		// "Return non-zero to accept the response, else zero to ignore it."
		return 1;
	}


	static int Read(void* pUserdata, const char* pszBuf, size_t nLen)
	{
		HTTPRequest* pThis = (HTTPRequest*) pUserdata;

		pThis->m_resultStream.write(pszBuf, nLen);

		// "Returns zero on success, or non-zero on error. [...] The request 
		// dispatch (or ne_read_response_block call) will fail with NE_ERROR; 
		// the session error string should have been set by the callback."
		//ne_session* pSession;
		//ne_set_error(pSession, "Foo %s", "bar");
		return 0;
	}

	std::ostringstream m_resultStream;
	HTTPSession& m_session;
	ne_request* m_pRequest;
};


#endif // NEON_H
