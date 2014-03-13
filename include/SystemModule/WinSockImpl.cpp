#include "WinSockImpl.h"
#include <assert.h>
ZHTTPBEGIN

/*
IWebConnector* CreateIWebConnector()
{
	CWinSock* sock = new CWinSock();
	return static_cast<IWebConnector*>(sock);
}
IWebListener* CreateIWebListener()
{
	CWinSock* sock = new CWinSock();
	return static_cast<IWebListener*>(sock);
}
*/

IWebListener* CreateListenerAt(const ZCharA* Port)
{
	CWinSock *socketRet = new CWinSock();
	ScopeGuard onExit([&] {
		socketRet->Release();
	});
	ZInt iResult = socketRet->Bind(Port);
	if (iResult < 0) return nullptr;
	iResult = socketRet->Listen();
	if (iResult < 0) return nullptr;
	onExit.Dismiss();
	return static_cast<IWebListener*>(socketRet);
}
IWebSocket* CreateConnetionTo(const ZCharA* Addr, const ZCharA* Port)
{
	CWinSock* socketRet = new CWinSock();
	ScopeGuard onExit([&] {
		socketRet->Release();
	});
	if (socketRet->Connect(Addr, Port) < 0) {
		return nullptr;
	}
	else {
		onExit.Dismiss();
		return static_cast<IWebSocket*>(socketRet);
	}
}
//construct
CWinSock::CWinSock()
{
	Init();
}
CWinSock::~CWinSock()
{
}
//IWebConnector
ZInt CWinSock::Connect(const ZCharA* Addr, const ZCharA* Port)
{
	struct addrinfo *result = nullptr, hints;
	ZInt iResult;
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	iResult = getaddrinfo(Addr, Port, &hints, &result);
	if (iResult != 0) {
		return -1;
	}
	ON_SCOPE_EXIT([&] {
		freeaddrinfo(result);
	});
	m_socket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (m_socket == INVALID_SOCKET) {
		return -1;
	}
	iResult = connect(m_socket, result->ai_addr, (int)result->ai_addrlen);
	if (iResult == SOCKET_ERROR) {
		closesocket(m_socket);
		m_socket = INVALID_SOCKET;
		return -1;
	}
	return 0;
}

//IWebListener
ZInt CWinSock::Bind(const ZCharA* Port)
{
	struct addrinfo	*tcpaddr = NULL, *ptr = NULL, hints;
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	int iResult = getaddrinfo(NULL, Port,
		&hints, &tcpaddr);
	if (iResult != 0) {
		return -1;
	}
	ON_SCOPE_EXIT([&] {
		freeaddrinfo(tcpaddr);
	});

	m_socket = socket(tcpaddr->ai_family, tcpaddr->ai_socktype, tcpaddr->ai_protocol);
	if (m_socket == INVALID_SOCKET) {
		return -1;
	}
	iResult = bind(m_socket, tcpaddr->ai_addr, tcpaddr->ai_addrlen);
	if (iResult == SOCKET_ERROR) {
		return -1;
	}
	return 0;
}
ZInt CWinSock::Listen()
{
	assert(m_socket != INVALID_SOCKET);
	if (listen(m_socket, SOMAXCONN) == SOCKET_ERROR) {
		return -1;
	}
	else {
		return 0;
	}
}
IWebSocket* CWinSock::Accept()
{
	assert(m_socket != INVALID_SOCKET);
	CWinSock* sockRet = new CWinSock();
	sockRet->m_socket = accept(m_socket, NULL, NULL);
	if (sockRet->m_socket != INVALID_SOCKET) {
		return static_cast<IWebSocket*>(sockRet);
	}
	else {
		return nullptr;
	}
}
//common function
void CWinSock::Release()
{
	if (m_socket != INVALID_SOCKET) {
		closesocket(m_socket);
		m_socket = INVALID_SOCKET;
	}
	WSACleanup();
	delete this;
}
ZInt CWinSock::Send(const ZCharA* content, ZInt length)
{
	assert(m_socket != INVALID_SOCKET);
	return send(m_socket, (char*)content, length, 0);
}
ZInt CWinSock::Recv(char* buff, ZULong length)
{
	assert(m_socket != INVALID_SOCKET);
	return recv(m_socket, buff, length, 0);
}
ZInt CWinSock::Shutdown()
{
	assert(m_socket != INVALID_SOCKET);
	return shutdown(m_socket, SD_SEND);
}
//private method
void CWinSock::Init()
{
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	
}

ZHTTPEND