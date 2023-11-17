#pragma once
#include <map>

class Game
{
public:
	static Game* Instance();
	void init();
	void update();
	bool hasWin();
	void updateCells(std::pair<int, int>, int);

	inline int getWinner() { return mWinner; };
	inline bool hasWinner() { return mWinner != 0; };
	inline std::map<std::pair<int, int>, int> getCells() { return mCells; };

private:
	Game();
	static Game* mInstance;

	//PlayerId for each game cell
	std::map<std::pair<int, int>, int> mCells;
	int mWinner;
	int mTurnPlayer;
};