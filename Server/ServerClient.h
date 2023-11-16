#pragma once
#include "Server.h"

#include "SFML/Graphics.hpp"
#include "Game.h"

class ServerClient : public Server {
protected:
	void handleClient(SOCKET clientSocket, const std::string& sessionID) override;

public:
	ServerClient();
	void init() override;
};