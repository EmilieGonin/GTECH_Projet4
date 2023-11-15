#pragma once

#undef UNICODE

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <mutex>
#include <vector>
#include <string>

// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")
// #pragma comment (lib, "Mswsock.lib")

#define DEFAULT_BUFLEN 512

class Server {

private:
    //Game* game = Game::Instance();

    void initWSA();
    void initSocket();
    void listenClient();
    void accepteClient();
    void shutdownClient(SOCKET clientSocket);
    void handleClient(SOCKET clientSocket, const std::string& sessionID);
    
    std::string generateSessionID() const;
    std::vector<SOCKET> clients; // Liste des sockets des clients connectés
    std::mutex clientsMutex; // Mutex pour protéger l'accès à la liste des clients

    WSADATA wsaData;

protected:
    int iResult;
    int iSendResult;
    int recvbuflen = DEFAULT_BUFLEN;

    char recvbuf[DEFAULT_BUFLEN];

    SOCKET ClientSocket = INVALID_SOCKET;
    SOCKET ListenSocket = INVALID_SOCKET;

    struct addrinfo* result = NULL;
    struct addrinfo hints;

    std::string mPort;

public:
    Server();
    ~Server();
    int start();
    // Ajoute d'autres méthodes et membres au besoin.
};
