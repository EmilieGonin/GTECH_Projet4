#include "JsonHandler.h"

JsonHandler::JsonHandler(std::pair<int, int> cell, int player)
{
	mJson["ErrorCode"] = 0;
	mJson["JsonType"] = REQUEST;
	mJson["Id"] = 1;
	mJson["Cell"] = cell;
	mJson["Player"] = player;

	mDump = mJson.dump();
}
JsonHandler::JsonHandler(int player)
{
	mJson["ErrorCode"] = 0;
	mJson["JsonType"] = REQUEST;
	mJson["Id"] = 2;
	mJson["Player"] = player;

	mDump = mJson.dump();
}

JsonHandler::JsonHandler(std::string jsonDump)
{
	json j = jsonDump;
}