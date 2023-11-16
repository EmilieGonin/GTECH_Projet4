#pragma once
#include "Server.h"

#include "SFML/Graphics.hpp"
#include "Game.h"

class ServerClient : public Server {
public:
	ServerClient();
	void init() override;
};