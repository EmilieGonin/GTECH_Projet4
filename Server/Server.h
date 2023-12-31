#pragma once

#define WIN32_LEAN_AND_MEAN
#define WM_SOCKET_EVENT (WM_USER + 1)
#define FD_READ_EVENT   FD_READ | FD_WRITE | FD_OOB | FD_ACCEPT | FD_CONNECT | FD_CLOSE
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define ADDRESS "192.168.1.72"
//#define ADDRESS "localhost"

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <mutex>
#include <string>
#include "JsonHandler.h" //temp
#include "Game.h" //temp

// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")
// #pragma comment (lib, "Mswsock.lib")

#define DEFAULT_BUFLEN 1000
#define WM_SOCKET (WM_USER + 1)


class Server {

private:
    //Game* game = Game::Instance();
    void shutdownClient(SOCKET clientSocket);
    virtual void initHWND();
    void initWSA();
    void initSocket();
    void listenClient();

    WSADATA wsaData;

    struct addrinfo* result = NULL;
    struct addrinfo hints;

protected:
    std::string mPort;
    std::string generateSessionID() const;
    int iResult;
    int iSendResult;
    int recvbuflen = DEFAULT_BUFLEN;

    //SOCKET ClientSocket = INVALID_SOCKET;
    SOCKET ListenSocket = INVALID_SOCKET;

    char recvbuf[DEFAULT_BUFLEN];
    virtual void init();
    HWND hWnd;

    std::map<SOCKET, std::string> mPlayers;
    std::map<SOCKET, std::string> mSpectate;
    std::map<SOCKET, std::string> mAllClient;

    //static Server* pServer;
    std::string mName;

    virtual void HandleReadEvent(WPARAM wParam);
    virtual void HandleCloseEvent(WPARAM wParam);
    //static LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

    bool mHasGameStarted;

public:
    Server();
    ~Server();
};