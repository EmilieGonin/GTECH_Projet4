#include "JsonHandler.h"

JsonHandler::JsonHandler() {}

JsonHandler::JsonHandler(std::map<std::pair<int, int>, int> cells, bool error)
{
	mJson["ErrorCode"] = error ? -1 : 0;
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

JsonHandler::JsonHandler(std::string playerId)
{
	mJson["ErrorCode"] = 0;
	mJson["JsonType"] = NOTIF;
	mJson["Id"] = 5;
	mJson["Player"] = playerId;

	mDump = mJson.dump();
}