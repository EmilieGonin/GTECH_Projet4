#include "ServerWeb.h"


ServerWeb::ServerWeb() { };

void ServerWeb::init()
{
	mPort = "8888";
	mName = "Serveur web -";

	Server::init();
}

void ServerWeb::initHWND()
{
	WNDCLASS wcb = { 0 };
	wcb.lpfnWndProc = WindowProc;
	wcb.hInstance = GetModuleHandle(NULL);
	wcb.lpszClassName = L"AsyncSelectWindowClassB";

	if (!RegisterClass(&wcb)) {
		printf("%s RegisterClassB failed: %d\n", mName.c_str(), GetLastError());
		return;
	}

	hWnd = CreateWindowEx(0, L"AsyncSelectWindowClassB", L"AsyncSelectWindowB", 0, 0, 0, 0, 0, HWND_MESSAGE, NULL, GetModuleHandle(NULL), NULL);
	if (hWnd == NULL) {
		printf("%s CreateWindowEx failed: %d\n", mName.c_str(), GetLastError());
		return;
	}

	SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));

	ShowWindow(hWnd, SW_NORMAL);
	UpdateWindow(hWnd);
	pServer = reinterpret_cast<Server*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

	printf("%s HWND created\n", mName.c_str());
}

void ServerWeb::handleClient() {
	//do {

	//	iResult = recv(ClientSocket, recvbuf, recvbuflen, 0);
	//	if (iResult > 0)
	//	{
	//		printf("request received: %s\n", recvbuf);
	//		// Analyser la requête HTTP
	//		std::string httpResponse = processHttpRequest();

	//		iSendResult = send(ClientSocket, httpResponse.c_str(), httpResponse.size(), 0);

	//		if (iSendResult == SOCKET_ERROR)
	//		{
	//			printf("send failed with error: %d\n", WSAGetLastError());
	//			closesocket(ClientSocket);
	//			WSACleanup();
	//			break; // Sortir de la boucle si l'envoi échoue
	//		}
	//	}
	//	else if (iResult == 0)
	//	{
	//		printf("Connection closing from server...\n");
	//		break; // Sortir de la boucle si la connexion est fermée
	//	}
	//	else
	//	{
	//		int error = WSAGetLastError();
	//		if (error != WSAEWOULDBLOCK) {
	//			// Gérer l'erreur, mais ne pas fermer la connexion en cas de WSAEWOULDBLOCK
	//			printf("recv failed with error: %d\n", error);
	//			break; // Sortir de la boucle pour d'autres erreurs que WSAEWOULDBLOCK
	//		}
	//	}
	//} while (iResult > 0);
}

void ServerWeb::accepteClient(SOCKET client)
{
	// Accept a client socket
	client = accept(ListenSocket, NULL, NULL);
	if (client == INVALID_SOCKET) {
		printf("%s accept failed with error: %d\n", mName.c_str(), WSAGetLastError());
		closesocket(ListenSocket);
		WSACleanup();
		return;
	}
	printf("%s Client accepted.\n", mName.c_str());

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