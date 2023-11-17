#pragma once
#include "Server.h"

class ServerWeb: public Server  {
private:
	void handleClient(SOCKET clientSocket, const std::string& sessionID) override;
	std::string processHttpRequest();

public:
	ServerWeb();
	void init() override;
};