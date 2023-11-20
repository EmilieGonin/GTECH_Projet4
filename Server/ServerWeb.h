#pragma once
#include "Server.h"
#include <string>

class ServerWeb : public Server {
private:
    void handleClient();
    void accepteClient() override;
    std::string processHttpRequest();
    std::string imagePath = "/game.png";

public:
    ServerWeb();
    void init() override;
};
