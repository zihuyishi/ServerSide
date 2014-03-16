#include "HttpServer.h"
#include <Windows.h>
#include <process.h>
#include <assert.h>

ZHTTPBEGIN
typedef struct tagParamStruct {
	IWebSocket* sock;
	CHttpServer* pThis;
}PARAMSTRUCT, *PPARAMSTRUCT;


//construct
CHttpServer::CHttpServer()
{
}
CHttpServer::~CHttpServer()
{}
ZInt CHttpServer::ListenAt(const ZCharA* Port)
{
	m_listener = CreateListenerAt(Port);
	if (m_listener == nullptr) {
		return -1;
	}
	do {
		IWebSocket* request = m_listener->Accept();
		if (request == nullptr) continue;
		HandleClient(request);
	} while (true);
	return 0;
}

void CHttpServer::HandleClient(IWebSocket* websocket)
{
	HANDLE hThread;
	ZUInt uiThread;
	PPARAMSTRUCT lpParam = new PARAMSTRUCT;
	lpParam->pThis = this;
	lpParam->sock = websocket;
	hThread = (HANDLE)_beginthreadex(
		NULL, 0, &ThisType::RequestThread, (void*)lpParam,
		0, &uiThread);
	if (hThread == nullptr) {

	}
	else {
		CloseHandle(hThread);
	}
}
ZUInt CHttpServer::RequestThread(void* lpParam)
{
	assert(lpParam != nullptr);
	PPARAMSTRUCT pStruct = (PPARAMSTRUCT)lpParam;
	ThisType* pThis = pStruct->pThis;
	IWebSocket* ISocket = pStruct->sock;
	delete pStruct;
	ZCharA* recvBuf = new ZCharA[pThis->m_bufSize];
	ZInt iResult = ISocket->Recv(recvBuf, pThis->m_bufSize);
	CRequestStruct request = ParseRequest(recvBuf);
	
	// do something to request
	
	return 0;
}
ZHTTPEND