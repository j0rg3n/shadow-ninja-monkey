#include "HTTPClient.h"

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


HTTPClient::HTTPClient() : m_socket(INVALID_SOCKET)
{
	InitNetwork();
}


HTTPClient::~HTTPClient()
{
	DisconnectSocket();
	DeinitNetwork();
}


bool HTTPClient::Connect(std::string sAddress, boost::uint32_t nPort)
{
	if(!ConnectSocket(sAddress, nPort))
	{
		return false;
	}

	return true;
}


void HTTPClient::Disconnect()
{
	DisconnectSocket();
}


bool HTTPClient::Get(std::string sPath, std::string& sResult)
{
	assert(m_socket != INVALID_SOCKET);

	printf("Query: %s\n", sPath.c_str());
	printf("Result: %s\n", sResult.c_str());

	std::ostringstream out;
	out << "GET " << sPath << " HTTP/1.1\r\n";

	// TODO: Add additional headers here.

	out << "\r\n";

	if(!Write(out.str().c_str()))
	{
		return false;
	}

	if(!Read(sResult))
	{
		return false;
	}

	return true;
}


// ----------------------------------------------------------------------------


bool HTTPClient::ConnectSocket(std::string sAddress, boost::uint32_t nPort)
{
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


void HTTPClient::DisconnectSocket()
{
	if(m_socket == INVALID_SOCKET)
	{
		return;
	}

	closesocket(m_socket);
}


bool HTTPClient::Write(std::string sMessage)
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


bool HTTPClient::Read(std::string& sMessage)
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


void HTTPClient::InitNetwork()
{
#ifdef _WINDOWS
	WSADATA wsaData;
	int nResult = WSAStartup(MAKEWORD(2,2), &wsaData);
	assert(nResult == 0);
#endif
}


void HTTPClient::DeinitNetwork()
{
#ifdef _WINDOWS
	WSACleanup();
#endif
}
