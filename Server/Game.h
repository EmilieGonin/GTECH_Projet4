#pragma once
#include <map>

class Game
{
public:
	static Game* Instance();
	void init();
	void update();
	bool hasWin();

private:
	Game();
	static Game* mInstance;

	//PlayerId for each game cell
	std::map<std::pair<int, int>, int> mCells;
};