#pragma once
#include "Server.h"

class ServerClient : public Server {
public:
	ServerClient();
	void init() override;
};