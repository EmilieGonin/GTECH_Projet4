#pragma once

#undef UNICODE

#define WIN32_LEAN_AND_MEAN

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
    void handleHeartbeats();
    void handleClient(SOCKET clientSocket, const std::string& sessionID);
    void initWSA();
    void initSocket();
    void listenClient();
    void accepteClient();

    WSADATA wsaData;
    int iResult;

    SOCKET ListenSocket = INVALID_SOCKET;
    SOCKET ClientSocket = INVALID_SOCKET;

    struct addrinfo* result = NULL;
    struct addrinfo hints;

    int iSendResult;
    char recvbuf[DEFAULT_BUFLEN];
    int recvbuflen = DEFAULT_BUFLEN;

    std::vector<SOCKET> clients; // Liste des sockets des clients connectés
    std::mutex clientsMutex; // Mutex pour protéger l'accès à la liste des clients


    struct HeartbeatInfo {
        SOCKET socket;
        std::string sessionID;
        std::chrono::steady_clock::time_point lastHeartbeatTime;
    };

    // Liste des clients avec informations de cœur
    std::vector<HeartbeatInfo> heartbeatClients;
    std::mutex heartbeatMutex; // Mutex pour protéger l'accès à la liste des clients

    // Fréquence des cœurs en millisecondes
    const int HEARTBEAT_INTERVAL = 3000; 

public:
    Server();
    ~Server();
    int start();
    // Ajoute d'autres méthodes et membres au besoin.

};
