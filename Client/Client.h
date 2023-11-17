#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <stdio.h>
#include <thread>

#pragma comment(lib, "Ws2_32.lib")

#define DEFAULT_PORT "1027"
#define ADDRESS "localhost"
#define DEFAULT_BUFLEN 512
#define WM_SOCKET (WM_USER + 1)


class Client
{
public:
	Client();
	~Client();
	int createInvisibleWindow();
	int initClientSocket();
	int connectClientServer();
	int clientSendData();
	int clientDisconnect();

	static LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT HandleWindowMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

	void HandleReadEvent(WPARAM wParam);
	void HandleAcceptEvent(WPARAM wParam);
	void HandleCloseEvent(WPARAM wParam);
private:
	int res;
	WSADATA wsaData;
	addrinfo* result = NULL, * connection = NULL, address;
	SOCKET ClientSocket = INVALID_SOCKET;

	const char* sendbuf = "close";
	int recvbuflen = DEFAULT_BUFLEN;
	char recvbuf[DEFAULT_BUFLEN];
	int iResult;
};

