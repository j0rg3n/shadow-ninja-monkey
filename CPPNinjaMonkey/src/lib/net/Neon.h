#pragma once
#ifndef NEON_H
#define NEON_H


#include <string>
#include "boost/integer_fwd.hpp"


// -----------------------------------------------------------------------------


struct HTTPSession
{
	static HTTPSession* CreateInstance(const char* pszScheme, const char* pszHostName, boost::uint32_t nPort);
	virtual ~HTTPSession() {}

	virtual const std::string& GetBoundAddress() const = 0;
	virtual boost::uint32_t GetBoundPort() const = 0;

	static std::string GetURLEscaped(const std::string& sText);
	static void WriteURLEscaped(std::ostream& out, const std::string& sText);
};


struct HTTPRequest
{
	static HTTPRequest* CreateInstance(HTTPSession& session, const char* pszMethod, const char* pszPath);
	virtual ~HTTPRequest() {};
	virtual bool Dispatch() = 0;
	virtual std::string GetResult() const = 0;
};


// -----------------------------------------------------------------------------


#endif // NEON_H
