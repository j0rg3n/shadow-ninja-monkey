#pragma once
#ifndef SOCKET_H
#define SOCKET_H


#include <string>
#include "boost/cstdint.hpp"


// -----------------------------------------------------------------------------


#if defined(_WINDOWS)
#include <WinSock2.h>
//typedef unsigned __int64 SOCKET;
#else
typedef unsigned int SOCKET;
#endif


// -----------------------------------------------------------------------------


class Socket
{
public:
	static const size_t READ_ERROR = 0xFFFFFFFF;

	Socket();
	Socket(SOCKET socket);
	~Socket();

	bool IsConnected() const;
	bool GetLocalEndPoint(std::string& sAddress, boost::uint32_t& nPort) const;
	bool Connect(std::string sAddress, boost::uint32_t nPort);
	bool Listen(boost::uint32_t nPort);
	bool Accept(Socket** ppConnection);
	void Disconnect();
	bool Write(const char* pBuffer, size_t nLength);
	size_t Read(char* pBuffer, size_t nLength);
	bool Write(std::string sMessage);
	bool Read(std::string& sMessage);

	static void InitNetwork();
	static void DeinitNetwork();

private:
	Socket(SOCKET socket, bool bSharedHandle);

	SOCKET m_socket;
	bool m_bSharedHandle;
	static const size_t RECEIVE_BUFFER_LENGTH = 1024;
};


#endif // SOCKET_H
