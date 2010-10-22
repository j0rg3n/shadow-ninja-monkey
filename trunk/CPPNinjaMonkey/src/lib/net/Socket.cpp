#include "Socket.h"
#include "diag/Trace.h"

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


Socket::Socket() : m_socket(INVALID_SOCKET), m_bSharedHandle(false)
{
}


Socket::Socket(SOCKET socket) : m_socket(socket), m_bSharedHandle(true)
{
}

	
Socket::~Socket()
{
	if (!m_bSharedHandle)
	{
		Disconnect();
	}
}


bool Socket::IsConnected() const
{
	// TODO: This is not good enough if m_bSharedHandle == true.
	return m_socket != INVALID_SOCKET;
}


bool Socket::GetLocalEndPoint(std::string& sAddress, boost::uint32_t& nPort) const
{
	sockaddr_in address;
	int nNameLen = sizeof(address);
	int nResult = ::getsockname(m_socket, (SOCKADDR*) &address, &nNameLen);
	if (nResult != 0)
	{
		TRACE("Failed getting local endpoint: 0x%08x", WSAGetLastError());
		return false;
	}

	assert(address.sin_family == AF_INET);
	nPort = ntohs(address.sin_port);
	sAddress.assign(inet_ntoa(address.sin_addr));
	
	return true;
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
		Disconnect();
		return false;
	}

	return true;
}


bool Socket::Listen(boost::uint32_t nPort)
{
	assert(m_socket == INVALID_SOCKET);

	m_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(m_socket == INVALID_SOCKET)
	{
		return false;
	}

	sockaddr_in address;
	address.sin_family = AF_INET;
	// Binding to loopback is maybe a bad idea.
	// TODO: Use ADDR_ANY?
	address.sin_addr.s_addr = inet_addr("127.0.0.1");
	address.sin_port = htons((u_short) nPort);

	int nResult = ::bind(m_socket, (SOCKADDR*) &address, sizeof(address));
	if(nResult != 0)
	{
		Disconnect();
		return false;
	}

	// "If set to SOMAXCONN, the underlying service provider responsible for 
	// socket s will set the backlog to a maximum reasonable value."
	// - ms-help://MS.VSCC.v90/MS.MSDNQTR.v90.en/winsock/winsock/listen_2.htm
	const int nBacklog = SOMAXCONN;
	nResult = listen(m_socket, nBacklog);
	if(nResult != 0)
	{
		Disconnect();
		return false;
	}

	return true;
}


bool Socket::Accept(Socket** ppConnection)
{
	sockaddr* pConnectionAddress = NULL;
	int *pnConnectionAddressLength = NULL;
	SOCKET incomingConnection = accept(m_socket, pConnectionAddress, pnConnectionAddressLength);
	
	// Note: After returning here, on errors, we must not access m_socket,
	// as it might have been cleared by another thread calling Disconnect.
	if(incomingConnection == INVALID_SOCKET)
	{
		*ppConnection = NULL;
		Disconnect();
		return false;
	}

	const bool HANDLE_OWNED = false;
	*ppConnection = new Socket(incomingConnection, HANDLE_OWNED);
	return true;
}


void Socket::Disconnect()
{
	closesocket(m_socket);
	m_socket = INVALID_SOCKET;
}


bool Socket::Write(const char* pBuffer, size_t nLength)
{
	const char* pszSendPos = pBuffer;
	const char* pszSendEnd = pBuffer + nLength;
	while(pszSendPos < pszSendEnd)
	{
		int nResult = send(m_socket, pszSendPos, pszSendEnd - pszSendPos, 0);
		if(nResult == SOCKET_ERROR)
		{
			Disconnect();
			return false;
		}

		pszSendPos += (size_t) nResult;
	}

	return true;
}


size_t Socket::Read(char* pBuffer, size_t nLength)
{
	int nResult = recv(m_socket, pBuffer, nLength, 0);
	if(nResult == SOCKET_ERROR)
	{
		Disconnect();
		return READ_ERROR;
	}

	return (size_t) nResult;
}


bool Socket::Write(std::string sMessage)
{
	return Write(sMessage.c_str(), sMessage.length());
}


bool Socket::Read(std::string& sMessage)
{
	char szBuffer[RECEIVE_BUFFER_LENGTH];
	size_t nLength = Read(szBuffer, RECEIVE_BUFFER_LENGTH);
	if(nLength == READ_ERROR)
	{
		return false;
	}

	sMessage.assign(szBuffer, nLength);
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


// -----------------------------------------------------------------------------


Socket::Socket(SOCKET socket, bool bSharedHandle) : m_socket(socket), m_bSharedHandle(bSharedHandle)
{ 
}
