#pragma once
#ifndef SOCKET_H
#define SOCKET_H


#include <string>
#include "boost/cstdint.hpp"


// -----------------------------------------------------------------------------


typedef unsigned int SOCKET;


// -----------------------------------------------------------------------------


class Socket
{
public:
	Socket();
	~Socket();

	bool IsConnected() const;
	bool Connect(std::string sAddress, boost::uint32_t nPort);
	bool Listen(boost::uint32_t nPort);
	bool Accept(Socket** ppConnection);
	void Disconnect();
	bool Write(std::string sMessage);
	bool Read(std::string& sMessage);

	static void InitNetwork();
	static void DeinitNetwork();

private:
	Socket(SOCKET socket);

	SOCKET m_socket;
	static const size_t RECEIVE_BUFFER_LENGTH = 1024;
};


#endif // SOCKET_H
