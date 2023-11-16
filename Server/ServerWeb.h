#pragma once
#include "Server.h"

class ServerWeb: public Server  {
private:
	void handleClientWeb(SOCKET clientSocket, const std::string& sessionID);
	std::string processHttpRequest(const std::string& httpRequest);

public:
	ServerWeb();
	void init() override;
};