#include "..\Common\ZDef.h"
#include "IWebConnect.h"
#include "HttpStruct.h"

#ifndef ZHTTP_HTTPSERVER_HEADER
#define ZHTTP_HTTPSERVER_HEADER

ZHTTPBEGIN
class CHttpServer :
	public UnCopyable
{
public:
	typedef CHttpServer ThisType;
	CHttpServer();
	~CHttpServer();
public:
	ZInt ListenAt(const ZCharA* Port);

private:
	void HandleClient(IWebSocket* websocket);
	static ZUInt __stdcall RequestThread(void* lpParam);
	
private:
	IWebListener*	m_listener = nullptr;
	ZUInt			m_bufSize = 2048;
};
ZHTTPEND
#endif