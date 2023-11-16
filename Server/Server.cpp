#include "ServerClient.h"
#include <thread>
#include <windows.h>
#include "../WindowsProject1/framework.h"


Server::Server()  {}
Server::~Server() {}

void Server::init()
{
	//game->init();
	//while (game->isOpen()) game->update();
	initHWND();
	initWSA();
	initSocket();
	listenClient();
	closesocket(ListenSocket);

	// cleanup
	closesocket(ClientSocket);
	WSACleanup();
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
}

void Server::initSocket()
{
	// Resolve the server address and port
	iResult = getaddrinfo(NULL, mPort.c_str(), &hints, &result);
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

int Server::initHWND()
{
	WNDCLASS wc = { 0 };
	wc.lpfnWndProc = &Server::WindowProc; 
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = GetModuleHandle(NULL);
	wc.lpszClassName = "AsyncSelectWindowClass";

	if (!RegisterClass(&wc)) {
		printf("RegisterClass failed: %d\n", GetLastError());
		return 1;
	}

	hWnd = CreateWindowEx(0, "AsyncSelectWindowClass", "AsyncSelectWindow", 0, 0, 0, 0, 0, HWND_MESSAGE, NULL, GetModuleHandle(NULL), NULL);
	if (hWnd == NULL) {
		printf("CreateWindowEx failed: %d\n", GetLastError());
		return 1;
	}

	SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));

	ShowWindow(hWnd, SW_HIDE);
	UpdateWindow(hWnd);
}

void Server::listenClient()
{
	
	iResult = bind(ListenSocket, result->ai_addr, static_cast<int>(result->ai_addrlen));
	if (iResult == SOCKET_ERROR) {
		printf("bind failed with error: %d\n", WSAGetLastError());
		freeaddrinfo(result);
		closesocket(ListenSocket);
		WSACleanup();
		return;
	}
	printf("Bind successful.\n");  

	freeaddrinfo(result);

	iResult = listen(ListenSocket, SOMAXCONN);
	if (iResult == SOCKET_ERROR) {
		printf("listen failed with error: %d\n", WSAGetLastError());
		closesocket(ListenSocket);
		WSACleanup();
		return;
	}
	printf("Server listening...\n");  

	accepteClient();
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


	WSAAsyncSelect(ClientSocket, hWnd, WM_SOCKET, FD_READ | FD_CLOSE);

	// Attribuer un identifiant de session au client
	std::string sessionID = generateSessionID();
	send(ClientSocket, sessionID.c_str(), sessionID.size(), 0);

	std::lock_guard<std::mutex> lock(clientsMutex);
	clients.push_back(ClientSocket);

	//handleClient(ClientSocket, sessionID);
	
}

LRESULT Server::WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) //static
{
	Server* pServer = reinterpret_cast<Server*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	if (pServer) return pServer->HandleWindowMessage(uMsg, wParam, lParam);
	
	if (uMsg == WM_SOCKET) 
	{
		switch (lParam) {
		case FD_READ:
			pServer->HandleReadEvent(wParam);
			break;
		case FD_ACCEPT:
			pServer->HandleAcceptEvent(wParam);
			break;
		case FD_CLOSE:
			pServer->HandleCloseEvent(wParam);
			break;
		default:
			// Gérer d'autres événements si nécessaire
			break;
		}
		return 0; // Indique que le message a été traité
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

LRESULT Server::HandleWindowMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) 
{	
	int requestID = static_cast<int>(wParam);
	std::string requestData(reinterpret_cast<const char*>(lParam));
	if (requestData.find("close") != std::string::npos)
	{
		printf("close connexion since you asked it");
		std::this_thread::sleep_for(std::chrono::milliseconds(5000));
		closesocket(ClientSocket);
		WSACleanup();
	}


	/*switch (uMsg) 
	{

	}*/

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

void Server::handleClient(UINT uMsg, WPARAM wParam, LPARAM lParam) {
	//do {

		if (iResult > 0)
		{

			printf("Bytes received: %d\n", recvbuf);

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
			//printf("Connection closing from server...\n");

		}
		/*else
		{
			printf("recv failed with error: %d\n", WSAGetLastError());
			closesocket(ClientSocket);
			WSACleanup();
		}*/

	//} while (true);
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

void Server::HandleReadEvent(WPARAM wParam) 
{
	// Traitement pour l'événement FD_READ
	printf("Read event\n" + wParam);
	iResult = recv(ClientSocket, recvbuf, recvbuflen, 0);


}

void Server::HandleAcceptEvent(WPARAM wParam) 
{
	// Traitement pour l'événement FD_ACCEPT
	printf("Accept event\n" + wParam);

}

void Server::HandleCloseEvent(WPARAM wParam) 
{
	// Traitement pour l'événement FD_CLOSE
	printf("Close event\n" + wParam);

}