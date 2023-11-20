#pragma once

#undef UNICODE

#define WIN32_LEAN_AND_MEAN
#define WM_SOCKET_EVENT (WM_USER + 1)
#define FD_READ_EVENT   FD_READ | FD_WRITE | FD_OOB | FD_ACCEPT | FD_CONNECT | FD_CLOSE
#define _WINSOCK_DEPRECATED_NO_WARNINGS

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
    virtual void handleClient(UINT uMsg, WPARAM wParam, LPARAM lParam);
    void initWSA();
    void initSocket();
    int initHWND();
    void listenClient();
    virtual void accepteClient();

    static LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    LRESULT HandleWindowMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

    void HandleReadEvent(WPARAM wParam);
    void HandleAcceptEvent(WPARAM wParam);
    void HandleCloseEvent(WPARAM wParam);

    WSADATA wsaData;

    struct addrinfo* result = NULL;
    struct addrinfo hints;

protected:
    std::string mPort;
    std::string generateSessionID() const;

    int iResult;
    int iSendResult;
    int recvbuflen = DEFAULT_BUFLEN;

    SOCKET ClientSocket = INVALID_SOCKET;
    SOCKET ListenSocket = INVALID_SOCKET;

    char recvbuf[DEFAULT_BUFLEN];
    virtual void init();
    HWND hWnd;

    std::vector<SOCKET> clients; // Liste des sockets des clients connect�s
    std::mutex clientsMutex; // Mutex pour prot�ger l'acc�s � la liste des clients

public:
    Server();
    ~Server();
    // Ajoute d'autres m�thodes et membres au besoin.
    void sendJson(std::string);

};
