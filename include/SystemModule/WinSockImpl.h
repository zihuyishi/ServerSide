#include "..\Common\ZDef.h"
#include "..\HttpModule\IWebConnect.h"

#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>

#pragma comment(lib, "Ws2_32.lib")

#ifndef HTTP_WINSOCKETIMPL_HEADER
#define HTTP_WINSOCKETIMPL_HEADER

ZHTTPBEGIN

class CWinSock :
	//public IWebConnector,
	public IWebListener,
	public IWebSocket,
	public UnCopyable
{
public:
	typedef CWinSock ThisType;
//construct
public:
	CWinSock();
	~CWinSock();

//IWebConnector
public:
	ZInt Connect(const ZCharA* Addr, const ZCharA* Port);

//IWebListener
public:
	ZInt Bind(const ZCharA* Port);
	ZInt Listen();
	IWebSocket* Accept();

//IWebSocket
public:
	ZInt Send(const ZCharA* content, ZInt length);
	ZInt Recv(char* buff, ZULong length);
	ZInt Shutdown();
	void Release();

private:
	void Init();
	ZInt createSocket();
private:
	SOCKET m_socket = INVALID_SOCKET;
};
ZHTTPEND
#endif