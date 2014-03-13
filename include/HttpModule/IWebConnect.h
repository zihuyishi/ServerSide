#include "..\Common\ZDef.h"

#ifndef HTTP_IWEBCONNECT_HEADER
#define HTTP_IWEBCONNECT_HEADER

ZHTTPBEGIN
struct IWebSocket
{
	virtual ZInt Recv(char* buff, ZULong length) = 0;
	virtual ZInt Send(const ZCharA* content, ZInt length) = 0;
	virtual ZInt Shutdown() = 0;
	virtual void Release() = 0;
};
struct IWebListener
{
	virtual IWebSocket* Accept() = 0;
	virtual void Release() = 0;
};
IWebSocket* CreateConnetionTo(const ZCharA* Addr, const ZCharA* Port);
IWebListener* CreateListenerAt(const ZCharA* Port);

/*
感觉下面两个接口没有必要了。不过先留着，等待以后实践的检验。
struct IWebConnector
{
	virtual ZInt Connect(const ZStringA& Addr, const ZStringA& Port) = 0;
	virtual void Release() = 0;
};
IWebConnector* CreateIWebConnector();

struct IWebListener
{
	virtual ZInt Bind(const ZStringA& Port) = 0;
	virtual ZInt Listen() = 0;
	virtual IWebSocket* Accept() = 0;
	virtual void Release() = 0;
};
IWebListener* CreateIWebListener();
*/
ZHTTPEND
#endif