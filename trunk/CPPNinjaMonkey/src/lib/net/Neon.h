#pragma once
#ifndef NEON_H
#define NEON_H

#include "neon/ne_session.h"
#include "neon/ne_request.h"


#include <iostream>


// -----------------------------------------------------------------------------


class NeonSession
{
public:
	NeonSession(const char* pszScheme, const char* pszHostName, boost::uint32_t nPort)
	{
		pSession = ne_session_create(pszScheme, pszHostName, nPort);
	}


	~NeonSession()
	{
		ne_session_destroy(pSession);
	}


	ne_session* GetInternalSession() { return pSession; }


private:
	ne_session* pSession;
};


class NeonRequest
{
public:
	NeonRequest(NeonSession& session, const char* pszMethod, const char* pszPath) : session(session)
	{
		char* pszEscapedPath = ne_path_escape(pszPath);
		pRequest = ne_request_create(session.GetInternalSession(), pszMethod, pszEscapedPath);
		free(pszEscapedPath);

		ne_add_response_body_reader(pRequest, Accept, Read, this);
	}


	~NeonRequest()
	{
		ne_request_destroy(pRequest);
	}


	bool Dispatch()
	{
		switch (ne_request_dispatch(pRequest))
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
			std::cerr << "An error occurred: " << ne_get_error(session.GetInternalSession()) << std::endl;
			return false;
		}
	}


private:
	static int Accept(void* pUserdata, ne_request* pRequest, const ne_status* pStatus)
	{
		NeonRequest* pThis = (NeonRequest*) pUserdata;

		// "Return non-zero to accept the response, else zero to ignore it."
		return 1;
	}


	static int Read(void* pUserdata, const char* pszBuf, size_t nLen)
	{
		NeonRequest* pThis = (NeonRequest*) pUserdata;

		std::cout.write(pszBuf, nLen);
		std::cout << std::endl;

		// "Returns zero on success, or non-zero on error. [...] The request 
		// dispatch (or ne_read_response_block call) will fail with NE_ERROR; 
		// the session error string should have been set by the callback."
		//ne_session* pSession;
		//ne_set_error(pSession, "Foo %s", "bar");
		return 0;
	}

	NeonSession& session;
	ne_request* pRequest;
};


#endif // NEON_H
