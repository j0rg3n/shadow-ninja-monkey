#pragma once
#ifndef HTTP_CLIENT_H
#define HTTP_CLIENT_H

#include <string>
#include "boost/cstdint.hpp"


// -----------------------------------------------------------------------------


class HTTPClient
{
public:
	HTTPClient();
	~HTTPClient();

	bool Connect(std::string sAddress, boost::uint32_t nPort);
	void Disconnect();

	bool Get(std::string sPath, std::string& sResult);

private:
	class Impl;

	Impl* pImpl;
};


// -----------------------------------------------------------------------------


#endif // HTTP_CLIENT_H
