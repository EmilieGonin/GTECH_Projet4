#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#define WM_SOCKET_EVENT (WM_USER + 1)
#define FD_READ_EVENT FD_READ | FD_WRITE | FD_OOB | FD_ACCEPT | FD_CONNECT | FD_CLOSE
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <stdio.h>
#include <thread>
#include "JsonHandler.h"

#pragma comment(lib, "Ws2_32.lib")

#define DEFAULT_PORT "1027"
#define ADDRESS "localhost"
#define DEFAULT_BUFLEN 1000
#define WM_SOCKET (WM_USER + 1)


class Client
{
public:
	Client();
	~Client();
	int init();
	int initHWND();
	int initSocket();
	int connectClientServer();
	int clientSendData(std::string);
	//int clientDisconnect();

	static LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT HandleWindowMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

	void HandleReadEvent(WPARAM wParam);
	void HandleCloseEvent(WPARAM wParam);

private:
	void sendJson(std::string json);
	void handleJson(std::string dump);
	void WriteData(std::string sessionID);
	json ReadData();
	void DeleteData();
	bool IsDataExist();


	int res;
	WSADATA wsaData;
	addrinfo* result = NULL, * connection = NULL, address;
	SOCKET ClientSocket = INVALID_SOCKET;

	const char* sendbuf = "close";
	int recvbuflen = DEFAULT_BUFLEN;
	char recvbuf[DEFAULT_BUFLEN];
	int iResult;
	HWND hWnd;
	std::string mPlayerId;
};

