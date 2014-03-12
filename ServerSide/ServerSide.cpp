// ServerSide.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include <assert.h>
#include <iostream>
#include <process.h>

#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>

#pragma comment(lib, "Ws2_32.lib")

void HandleSocket(SOCKET s);
using namespace ZHttp;
int _tmain(int argc, _TCHAR* argv[])
{
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	ON_SCOPE_EXIT([&] {
		WSACleanup();
	});

	SOCKET					s = INVALID_SOCKET;
	struct addrinfo			*tcpaddr = NULL, *ptr = NULL, hints;
	int						port = 5150;

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	int iResult = getaddrinfo(NULL, "5150", &hints, &tcpaddr);
	if (iResult != 0) {
		o_normal_error("getaddrinfo failed: %d\n");
	}
	ON_SCOPE_EXIT([&] {
		freeaddrinfo(tcpaddr);
	});
	s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (s == INVALID_SOCKET) {
		o_normal_error("Error at socket");
	}
	ON_SCOPE_EXIT([&] {
		closesocket(s);
	});
	iResult = bind(s, tcpaddr->ai_addr, tcpaddr->ai_addrlen);
	if (iResult == SOCKET_ERROR) {
		o_normal_error("bind failed");
	}
	if (listen(s, SOMAXCONN) == SOCKET_ERROR) {
		o_normal_error("Listen failed with error");
	}
	SOCKET ClientSocket = INVALID_SOCKET;
	do {
		ClientSocket = accept(s, NULL, NULL);
		if (ClientSocket == INVALID_SOCKET) {
			break;
		}
		HandleSocket(ClientSocket);
	} while (true);
	return 0;
}
struct SOCKETPARAM {
	SOCKET s;
};
unsigned int WINAPI HandleThread(void* lpParam)
{
	assert(lpParam != nullptr);
	SOCKET s = ((SOCKETPARAM*)lpParam)->s;
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
	iResult = recv(s, recvbuf, recvbuflen, 0);
	if (iResult > 0) {
		recvbuf[iResult] = 0;
		std::cout << recvbuf << std::endl;
		recvbuf[0] = 0;
	}
	else if (iResult == 0) {

	}
	else {
	}
	const char* header = "HTTP/1.1 404 NotFound\r\n"
				"Data:Mon 15\r\n"
				"Server:Apache / 7.0\r\n"
				"Content-Type:text/html\r\n"
				"Content - length:71\r\n"
				"\r\n"
				"<html><head><title>NotFound</title></head><body>Not Found</body></html>\r\n"
				"\r\n";

	send(s, header, strlen(header), 0);
	shutdown(s, SD_SEND);
	closesocket(s);
	return 0;
}
void HandleSocket(SOCKET s)
{
	assert(s != INVALID_SOCKET);
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