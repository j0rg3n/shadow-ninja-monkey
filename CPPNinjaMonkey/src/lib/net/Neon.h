#pragma once
#ifndef NEON_H
#define NEON_H


#include <string>
#include "boost/integer_fwd.hpp"


// -----------------------------------------------------------------------------


class HTTPSession
{
public:
	class Impl;

	HTTPSession(const char* pszScheme, const char* pszHostName, boost::uint32_t nPort);
	~HTTPSession();
	void* GetInternalSession();
	static std::string GetURLEscaped(const std::string& sText);
	static void WriteURLEscaped(std::ostream& out, const std::string& sText);

private:
	Impl* pImpl;
};


class HTTPRequest
{
public:
	class Impl;

	HTTPRequest(HTTPSession& session, const char* pszMethod, const char* pszPath);
	~HTTPRequest();
	bool Dispatch();
	std::string GetResult() const;

private:
	Impl* pImpl;
};


// -----------------------------------------------------------------------------


#endif // NEON_H
