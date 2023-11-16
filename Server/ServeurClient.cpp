#include "ServerClient.h"

ServerClient::ServerClient()
{
	
};

void ServerClient::init()
{
	mPort = "1027";
	Server::init();
}