#pragma once
#ifndef HTTP_CLIENT_H
#define HTTP_CLIENT_H

#include <string>
#include "boost/cstdint.hpp"

typedef unsigned int SOCKET;

class HTTPClient
{
public:
	HTTPClient();
	~HTTPClient();

	bool Connect(std::string sAddress, boost::uint32_t nPort);
	void Disconnect();

	bool Get(std::string sPath, std::string& sResult);

private:
	bool ConnectSocket(std::string sAddress, boost::uint32_t nPort);
	void DisconnectSocket();

	bool Write(std::string sMessage);
	bool Read(std::string& sMessage);

	static void InitNetwork();
	static void DeinitNetwork();

	std::string m_sURL;
	SOCKET m_socket;
	static const size_t RECEIVE_BUFFER_LENGTH = 1024;
};

#endif // HTTP_CLIENT_H
