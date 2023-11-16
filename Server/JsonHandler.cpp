#include "JsonHandler.h"

JsonHandler::JsonHandler(std::map<std::pair<int, int>, struct cell> cells)
{
	mJson["JsonType"] = RESPONSE;
	mJson["Id"] = 3;
	//mJson["Cells"] = cells;
}