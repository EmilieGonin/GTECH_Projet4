#pragma once
#include "Server.h"

class ServerClient : public Server {
protected:
	void handleClient(SOCKET clientSocket, const std::string& sessionID) override;
	void accepteClient() override;

public:
	ServerClient();
	void init() override;
};