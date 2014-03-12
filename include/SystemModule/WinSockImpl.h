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
	public IWebSender,
	public IWebListener
{
//IWebSender
public:
	ZInt Connect(const ZString& Addr, const ZString& Port);
	ZInt Send(const ZString& content);
//IWebListener
public:
	ZInt Bind(const ZString& Port);
	ZInt Listen();

public:
	ZString Recv();
	void Release();
private:
	SOCKET m_socket;
};
ZHTTPEND
#endif