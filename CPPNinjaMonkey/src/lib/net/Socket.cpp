#include "Socket.h"

#ifdef _WINDOWS
#include <winsock2.h>
#include <ws2tcpip.h>
#endif

#include <cassert>
#include <sstream>


// ----------------------------------------------------------------------------


using namespace boost;
using namespace std;


// ----------------------------------------------------------------------------


Socket::Socket() : m_socket(INVALID_SOCKET)
{
}


Socket::~Socket()
{
	Disconnect();
}


bool Socket::Connect(std::string sAddress, boost::uint32_t nPort)
{
	assert(m_socket == INVALID_SOCKET);

	addrinfo hints;
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	char szPort[32];
	_itoa(nPort, szPort, 10);

	addrinfo *pIPAddress = NULL;
	int nResult = getaddrinfo(sAddress.c_str(), szPort, &hints, &pIPAddress);
	if(nResult != 0)
	{
		return false;
	}

	m_socket = socket(pIPAddress->ai_family, pIPAddress->ai_socktype, pIPAddress->ai_protocol);
	if(m_socket == INVALID_SOCKET)
	{
		freeaddrinfo(pIPAddress);
		return false;
	}

	nResult = connect(m_socket, pIPAddress->ai_addr, (int) pIPAddress->ai_addrlen);
	freeaddrinfo(pIPAddress);

	if(nResult != 0)
	{
		closesocket(m_socket);
		return false;
	}

	return true;
}


void Socket::Disconnect()
{
	if(m_socket == INVALID_SOCKET)
	{
		return;
	}

	closesocket(m_socket);
}


bool Socket::Write(std::string sMessage)
{
	const char* pszSendPos = sMessage.c_str();
	const char* pszSendEnd = sMessage.c_str() + sMessage.length();
	while(pszSendPos < pszSendEnd)
	{
		int nResult = send(m_socket, pszSendPos, pszSendEnd - pszSendPos, 0);
		if(nResult == SOCKET_ERROR)
		{
			return false;
		}

		pszSendPos += (size_t) nResult;
	}

	return true;
}


bool Socket::Read(std::string& sMessage)
{
	char szBuffer[RECEIVE_BUFFER_LENGTH];
	int nResult = recv(m_socket, szBuffer, RECEIVE_BUFFER_LENGTH, 0);
	if(nResult == SOCKET_ERROR)
	{
		return false;
	}

	sMessage.assign(szBuffer, (size_t) nResult);
	return true;
}


void Socket::InitNetwork()
{
#ifdef _WINDOWS
	WSADATA wsaData;
	int nResult = WSAStartup(MAKEWORD(2,2), &wsaData);
	assert(nResult == 0);
#endif
}


void Socket::DeinitNetwork()
{
#ifdef _WINDOWS
	WSACleanup();
#endif
}
