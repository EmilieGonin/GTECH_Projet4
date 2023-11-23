#include "ServerClient.h"
#include <thread>
#include <windows.h>

Server::Server() {}
Server::~Server() {}

void Server::init()
{
	initHWND();
	initWSA();
	initSocket();
	listenClient();

	// cleanup
	//closesocket(ClientSocket);
	//WSACleanup();
}

void Server::initHWND() {}

void Server::initWSA()
{
	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) printf("%s WSAStartup failed with error: %d\n", mName.c_str(), iResult);

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
}

void Server::initSocket()
{
	// Resolve the server address and port
	iResult = getaddrinfo(ADDRESS, mPort.c_str(), &hints, &result);
	if (iResult != 0) {
		printf("%s getaddrinfo failed with error: %d\n", mName.c_str(), iResult);
		WSACleanup();
		return;
	}

	// Create a SOCKET for the server to listen for client connections.
	ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (ListenSocket == INVALID_SOCKET) {
		printf("%s socket failed with error: %ld\n", mName.c_str(), WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup();
		return;
	}

	printf("%s Socket initialized\n", mName.c_str());
}


void Server::listenClient()
{
	iResult = bind(ListenSocket, result->ai_addr, static_cast<int>(result->ai_addrlen));
	if (iResult == SOCKET_ERROR) {
		printf("%s bind failed with error: %d\n", mName.c_str(), WSAGetLastError());
		freeaddrinfo(result);
		closesocket(ListenSocket);
		WSACleanup();
		return;
	}
	printf("%s Bind successful.\n", mName.c_str());

	sockaddr_in serverAddr;
	int len = sizeof(serverAddr);
	if (getsockname(ListenSocket, reinterpret_cast<sockaddr*>(&serverAddr), &len) == 0) {
		char ipAddress[INET_ADDRSTRLEN];
		if (inet_ntop(AF_INET, &serverAddr.sin_addr, ipAddress, INET_ADDRSTRLEN) != nullptr) {
			printf("%s Server bound to IP address: %s\n", mName.c_str(), ipAddress);
		}
	}

	freeaddrinfo(result);

	iResult = listen(ListenSocket, SOMAXCONN);
	if (iResult == SOCKET_ERROR) {
		printf("%s listen failed with error: %d\n", mName.c_str(), WSAGetLastError());
		closesocket(ListenSocket);
		WSACleanup();
		return;
	}

	printf("%s Listening for clients...\n", mName.c_str());
	WSAAsyncSelect(ListenSocket, hWnd, WM_SOCKET, FD_ACCEPT | FD_CLOSE);

	MSG msg;
	/*while (GetMessage(&msg, hWnd, 0, 0))
	{
		printf("%s Getting message\n", mName.c_str());
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}*/
	while (true)
	{
		while (PeekMessage(&msg, hWnd, 0, 0, PM_REMOVE)) {
			//printf("%s Getting message\n", mName.c_str());
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
}

void Server::shutdownClient(SOCKET clientSocket)
{
	iResult = shutdown(clientSocket, SD_SEND);
	if (iResult == SOCKET_ERROR) {
		printf("%s shutdown failed with error: %d\n", mName.c_str(), WSAGetLastError());
		closesocket(clientSocket);
		WSACleanup();
	}
	printf("shutdown client");

	// Attendre quelques instants pour permettre la fermeture propre du socket
	std::this_thread::sleep_for(std::chrono::milliseconds(500));

	mPlayers.erase(clientSocket);

	//mPlayers.erase(std::remove_if(mPlayers.begin(), mPlayers.end(),
		//[clientSocket](SOCKET s) { return s == clientSocket; }), mPlayers.end());

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

void Server::HandleReadEvent(WPARAM socket) {}
void Server::HandleCloseEvent(WPARAM wParam) {}