#include "JsonHandler.h"

JsonHandler::JsonHandler(std::map<std::pair<int, int>, int> cells)
{
	mJson["ErrorCode"] = 0;
	mJson["JsonType"] = RESPONSE;
	mJson["Id"] = 3;
	mJson["Cells"] = cells;
}