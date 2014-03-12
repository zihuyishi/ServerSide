#include "..\Common\ZDef.h"

#ifndef HTTP_IWEBCONNECT_HEADER
#define HTTP_IWEBCONNECT_HEADER

ZHTTPBEGIN

struct IWebSender
{
	virtual ZInt Connect(const ZString& Addr, const ZString& Port) = 0;
	virtual ZInt Send(const ZString& content) = 0;
	virtual ZString Recv() = 0;
	virtual void Release() = 0;
};
IWebSender* CreateIWebSender();

struct IWebListener
{
	virtual ZInt Bind(const ZString& Port) = 0;
	virtual ZInt Listen() = 0;
	virtual ZString Recv() = 0;
	virtual void Release() = 0;
};
IWebListener* CreateIWebListener();

ZHTTPEND
#endif