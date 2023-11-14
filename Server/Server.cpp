#include "Server.h"
#include <thread>


Server::Server() {}
Server::~Server() {}


int __cdecl main(void)
{
	Server server;
	server.start();
}

int Server::start()
{
	game->init();
	//while (game->isOpen()) game->update();

	initWSA();
	initSocket();

	listenClient();
	closesocket(ListenSocket);

	// cleanup
	closesocket(ClientSocket);
	WSACleanup();

	return 0;
}



void Server::initWSA()
{
	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("WSAStartup failed with error: %d\n", iResult);
	}

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;
}

void Server::initSocket()
{
	// Resolve the server address and port
	iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
	if (iResult != 0) {
		printf("getaddrinfo failed with error: %d\n", iResult);
		WSACleanup();
	}

	// Create a SOCKET for the server to listen for client connections.
	ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (ListenSocket == INVALID_SOCKET) {
		printf("socket failed with error: %ld\n", WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup();
	}
}

void Server::listenClient()
{
	std::thread heartbeatThread(&Server::handleHeartbeats, this);
	heartbeatThread.detach();

	iResult = bind(ListenSocket, result->ai_addr, static_cast<int>(result->ai_addrlen));
	if (iResult == SOCKET_ERROR) {
		printf("bind failed with error: %d\n", WSAGetLastError());
		freeaddrinfo(result);
		closesocket(ListenSocket);
		WSACleanup();
		return;
	}
	printf("Bind successful.\n");  // Log ajouté

	freeaddrinfo(result);

	iResult = listen(ListenSocket, SOMAXCONN);
	if (iResult == SOCKET_ERROR) {
		printf("listen failed with error: %d\n", WSAGetLastError());
		closesocket(ListenSocket);
		WSACleanup();
		return;
	}
	printf("Server listening...\n");  // Log ajouté

	while (clients.size() < 2)
	{
		accepteClient();
	}
}

void Server::accepteClient()
{
	// Accept a client socket
	ClientSocket = accept(ListenSocket, NULL, NULL);
	if (ClientSocket == INVALID_SOCKET) {
		printf("accept failed with error: %d\n", WSAGetLastError());
		closesocket(ListenSocket);
		WSACleanup();
		return;
	}

	printf("Client accepted.\n"); 

	// Attribuer un identifiant de session au client
	std::string sessionID = generateSessionID();
	send(ClientSocket, sessionID.c_str(), sessionID.size(), 0);

	std::lock_guard<std::mutex> lock(clientsMutex);
	clients.push_back(ClientSocket);

	// Ajouter le client à la liste des cœurs
	std::lock_guard<std::mutex> heartbeatLock(heartbeatMutex);
	heartbeatClients.push_back({ ClientSocket, sessionID, std::chrono::steady_clock::now() });

	handleClient(ClientSocket, sessionID);
}

void Server::handleClient(SOCKET clientSocket, const std::string& sessionID) {
	do {

		iResult = recv(ClientSocket, recvbuf, recvbuflen, 0);
		if (iResult > 0)
		{
			printf("Bytes received: %s\n", sessionID.c_str(), recvbuf);

			// Echo the buffer back to the sender
			iSendResult = send(ClientSocket, recvbuf, iResult, 0);
			if (iSendResult == SOCKET_ERROR)
			{
				printf("send failed with error: %d\n", WSAGetLastError());
				closesocket(ClientSocket);
				WSACleanup();
			}
			printf("Bytes sent: %d\n", iSendResult);
		}
		else if (iResult == 0)
		{
			printf("Connection closing from server...\n");

		}
		else
		{
			printf("recv failed with error: %d\n", WSAGetLastError());
			closesocket(ClientSocket);
			WSACleanup();
		}

		// Met à jour le temps du dernier cœur pour le client
		std::lock_guard<std::mutex> heartbeatLock(heartbeatMutex);
		auto it = std::find_if(heartbeatClients.begin(), heartbeatClients.end(),
			[clientSocket](const HeartbeatInfo& info) { return info.socket == clientSocket; });
		if (it != heartbeatClients.end()) {
			it->lastHeartbeatTime = std::chrono::steady_clock::now();
		}

	} while (iResult > 0);
}

void Server::shutdownClient(SOCKET clientSocket)
{
	iResult = shutdown(ClientSocket, SD_SEND);
	if (iResult == SOCKET_ERROR) {
		printf("shutdown failed with error: %d\n", WSAGetLastError());
		closesocket(ClientSocket);
		WSACleanup();
	}

	// Attendre quelques instants pour permettre la fermeture propre du socket
	std::this_thread::sleep_for(std::chrono::milliseconds(500));

	clients.erase(std::remove_if(clients.begin(), clients.end(),
		[clientSocket](SOCKET s) { return s == clientSocket; }), clients.end());

	closesocket(clientSocket);

}

std::string Server::generateSessionID() const {
	// Implémente une logique pour générer un identifiant de session unique
	// Dans un contexte de production, tu pourrais utiliser une bibliothèque dédiée à la génération d'UUID ou d'autres méthodes de génération d'identifiants uniques.
	// Pour cette démo, un simple timestamp pourrait suffire.
	auto now = std::chrono::system_clock::now();
	auto timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();

	return "SessionID_" + std::to_string(timestamp);
}

void Server::handleHeartbeats() {
	while (true) {
		std::this_thread::sleep_for(std::chrono::milliseconds(HEARTBEAT_INTERVAL));

		// Vérifie les cœurs et déconnecte les clients qui n'ont pas répondu
		std::lock_guard<std::mutex> heartbeatLock(heartbeatMutex);
		auto currentTime = std::chrono::steady_clock::now();

		for (auto it = heartbeatClients.begin(); it != heartbeatClients.end();) {
			auto timeElapsed = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - it->lastHeartbeatTime).count();
			if (timeElapsed > HEARTBEAT_INTERVAL * 2) { // Si le temps écoulé est supérieur à deux fois l'intervalle de cœur
				printf("Client %s has disconnected due to lack of heartbeat.\n", it->sessionID.c_str());

				// Retirer le client des listes
				closesocket(it->socket);
				clients.erase(std::remove_if(clients.begin(), clients.end(),
					[socket = it->socket](SOCKET s) { return s == socket; }), clients.end());
				it = heartbeatClients.erase(it);
			}
			else {
				++it;
			}
		}
	}
}