#pragma once
#include "Server.h"

class ServerWeb: public Server  {
private:
	void handleClient();
	std::string processHttpRequest();
	void accepteClient() override;

public:
	ServerWeb();
	void init() override;
};