#include "JsonHandler.h"

JsonHandler::JsonHandler(std::map<std::pair<int, int>, int> cells)
{
	mJson["ErrorCode"] = 0;
	mJson["JsonType"] = RESPONSE;
	mJson["Id"] = 3;
	mJson["Cells"] = cells;

	mDump = mJson.dump();
}

JsonHandler::JsonHandler(std::pair<int, int> cell, int player)
{
	mJson["ErrorCode"] = 0;
	mJson["JsonType"] = REQUEST;
	mJson["Id"] = 1;
	mJson["Cell"] = cell;
	mJson["Player"] = player;

	mDump = mJson.dump();
}

JsonHandler::JsonHandler(std::string jsonDump)
{
	json j = jsonDump;
}