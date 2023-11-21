#include "JsonHandler.h"

JsonHandler::JsonHandler(std::map<std::pair<int, int>, int> cells, int error)
{
	mJson["ErrorCode"] = error;
	mJson["JsonType"] = RESPONSE;
	mJson["Id"] = 3;
	mJson["Cells"] = cells;

	mDump = mJson.dump();
}

JsonHandler::JsonHandler(std::map<std::pair<int, int>, int> cells, int player)
{
	mJson["ErrorCode"] = 0;
	mJson["JsonType"] = NOTIF;
	mJson["Id"] = 4;
	mJson["Cells"] = cells;
	mJson["Player"] = player;

	mDump = mJson.dump();
}

JsonHandler::JsonHandler(std::string jsonDump)
{
	json j = jsonDump;
}