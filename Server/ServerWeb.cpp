#include "ServerWeb.h"

ServerWeb::ServerWeb() { };

void ServerWeb::init()
{
	mPort = "8888";


	WNDCLASS wcb = { 0 };
	wcb.lpfnWndProc = WindowProc;
	wcb.hInstance = GetModuleHandle(NULL);
	wcb.lpszClassName = L"AsyncSelectWindowClassB";



	if (!RegisterClass(&wcb)) {
		printf("RegisterClassB failed: %d\n", GetLastError());
		return;
	}

	hWnd = CreateWindowEx(0, L"AsyncSelectWindowClassB", L"AsyncSelectWindowB", 0, 0, 0, 0, 0, HWND_MESSAGE, NULL, GetModuleHandle(NULL), NULL);
	if (hWnd == NULL) {
		printf("CreateWindowEx failed: %d\n", GetLastError());
		return;
	}

	SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));

	ShowWindow(hWnd, SW_NORMAL);
	UpdateWindow(hWnd);
	pServer = reinterpret_cast<Server*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

	printf("HWND created\n");

	Server::init();
}

void ServerWeb::handleClient() {
	do {

		iResult = recv(ClientSocket, recvbuf, recvbuflen, 0);
		if (iResult > 0)
		{
			printf("request received: %s\n", recvbuf);
			// Analyser la requ�te HTTP
			std::string httpResponse = processHttpRequest();

			iSendResult = send(ClientSocket, httpResponse.c_str(), httpResponse.size(), 0);

			if (iSendResult == SOCKET_ERROR)
			{
				printf("send failed with error: %d\n", WSAGetLastError());
				closesocket(ClientSocket);
				WSACleanup();
				break; // Sortir de la boucle si l'envoi �choue
			}
		}
		else if (iResult == 0)
		{
			printf("Connection closing from server...\n");
			break; // Sortir de la boucle si la connexion est ferm�e
		}
		else
		{
			int error = WSAGetLastError();
			if (error != WSAEWOULDBLOCK) {
				// G�rer l'erreur, mais ne pas fermer la connexion en cas de WSAEWOULDBLOCK
				printf("recv failed with error: %d\n", error);
				break; // Sortir de la boucle pour d'autres erreurs que WSAEWOULDBLOCK
			}
		}
	} while (iResult > 0);
}

void ServerWeb::accepteClient()
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

	handleClient();
}

std::string ServerWeb::processHttpRequest()
{
	std::ifstream file(imagePath, std::ios::binary);

	std::ostringstream imageStream;
	imageStream << file.rdbuf();

	std::string imageData = imageStream.str();
	std::string base64 = base64_encode(imageData);

	std::string html = "HTTP / 1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<h1>Tic-Tac-Toe</h1><img src=\"data:image/png;base64," + base64 + "\" />";

	return html;
}