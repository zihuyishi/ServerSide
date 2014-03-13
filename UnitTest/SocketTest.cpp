#include "SocketTest.h"
#include <tchar.h>
#include <Windows.h>
#include <assert.h>
#include <process.h>
using namespace ZHttp;
struct SOCKETPARAM {
	IWebSocket* s;
};
unsigned int WINAPI HandleThread(void* lpParam)
{
	assert(lpParam != nullptr);
	IWebSocket* s = ((SOCKETPARAM*)lpParam)->s;
	delete lpParam;
	int iResult = 0;
	int recvbuflen = 65536;
	char *recvbuf = new char[recvbuflen];
	ScopeGuard onExit([&] {
		if (recvbuf != nullptr) {
			delete[] recvbuf;
			recvbuf = nullptr;
		}
	});
	do {
		iResult = s->Recv(recvbuf, 1);
		if (iResult > 0) {
			recvbuf[iResult] = 0;
			std::cout << "recv buffer: " << recvbuf << std::endl;
			//s->Send(recvbuf, iResult);
			recvbuf[0] = 0;
		}
		else if (iResult == 0) {

		}
		else {
		}
	} while (true);

	s->Shutdown();
	s->Release();
	return 0;
}
void Test1()
{
	IWebSocket* connect = ZHttp::CreateConnetionTo("127.0.0.1", "5150");
	char buffer[1024];
	for (auto i : { "hi", "zeze", "hehe", "dafsdf" }) {
		connect->Send(i, strlen(i));
		/*
		int iResult = connect->Recv(buffer, 1024);
		buffer[iResult] = 0;
		std::cout << "sender buffer:" << buffer << std::endl;
		*/
	}
	connect->Release();

}
void HandleSocket(IWebSocket* s)
{
	assert(s != nullptr);
	HANDLE hThread;
	unsigned int dwThread;
	SOCKETPARAM* lpParam = new SOCKETPARAM;
	lpParam->s = s;
	hThread = (HANDLE)_beginthreadex(
		NULL,
		0,
		&HandleThread,
		(void*)lpParam,
		0,
		&dwThread);
	CloseHandle(hThread);
}

ZUInt __stdcall Test2Thread(void* lpParam)
{
	IWebListener* listener = ZHttp::CreateListenerAt("5150");
	do {
		IWebSocket* iwebsocket = listener->Accept();
		if (iwebsocket == nullptr) {
			break;
		}
		HandleSocket(iwebsocket);
	} while (true);
	listener->Release();
	return 0;
}
HANDLE Test2()
{
	HANDLE hThread;
	ZUInt dwThread;
	hThread = (HANDLE)_beginthreadex(NULL,
		0,
		&Test2Thread,
		NULL,
		0,
		&dwThread);
	return hThread;
}
void SocketTest()
{
	HANDLE hThread = Test2();
	Test1();
	WaitForSingleObject(hThread, INFINITE);
	CloseHandle(hThread);
}