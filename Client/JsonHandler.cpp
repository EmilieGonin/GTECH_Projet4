#include "JsonHandler.h"

JsonHandler::JsonHandler() {}

JsonHandler::JsonHandler(std::pair<int, int> cell, std::string player)
{
	mJson["ErrorCode"] = 0;
	mJson["JsonType"] = REQUEST;
	mJson["Id"] = 1;
	mJson["Cell"] = cell;
	mJson["Player"] = player;

	mDump = mJson.dump();
}
JsonHandler::JsonHandler(std::string player)
{
	mJson["ErrorCode"] = 0;
	mJson["JsonType"] = REQUEST;
	mJson["Id"] = 2;
	mJson["Player"] = player;

	mDump = mJson.dump();
}