#pragma once

#undef UNICODE

#define WIN32_LEAN_AND_MEAN
#define WM_SOCKET_EVENT (WM_USER + 1)
#define FD_READ_EVENT   FD_READ | FD_WRITE | FD_OOB | FD_ACCEPT | FD_CONNECT | FD_CLOSE


#include "SFML/Graphics.hpp"

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include "Game.h"
#include <mutex>


// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")
// #pragma comment (lib, "Mswsock.lib")

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "1027"


class Server {

private:
    //Game* game = Game::Instance();
    void shutdownClient(SOCKET clientSocket);
    std::string generateSessionID() const;
    void handleClient(SOCKET clientSocket, const std::string& sessionID);
    void initWSA();
    void initSocket();
    int initHWND();
    void listenClient();
    void accepteClient();

    static LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    //LRESULT HandleWindowMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

    HWND hWnd;

    WSADATA wsaData;

    SOCKET ListenSocket = INVALID_SOCKET;

    struct addrinfo* result = NULL;
    struct addrinfo hints;

    std::vector<SOCKET> clients; // Liste des sockets des clients connectés
    std::mutex clientsMutex; // Mutex pour protéger l'accès à la liste des clients

protected:
    std::string mPort = "1027";

    int iResult;
    int iSendResult;
    int recvbuflen = DEFAULT_BUFLEN;

    SOCKET ClientSocket = INVALID_SOCKET;

    char recvbuf[DEFAULT_BUFLEN];
    virtual void init();

public:
    Server();
    ~Server();
    // Ajoute d'autres méthodes et membres au besoin.

};
