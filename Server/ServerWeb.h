#pragma once
#include "Server.h"
#include <string>
#include <fstream>
#include <sstream>
#include "base64.h"

class ServerWeb : public Server {
private:
    void handleClient();
    void accepteClient(SOCKET) override;
    std::string processHttpRequest();
    std::string imagePath = "game.png";

public:
    ServerWeb();
    void init() override;
};
