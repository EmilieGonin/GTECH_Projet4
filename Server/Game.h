#pragma once
#include <SFML/Graphics.hpp>
#include <map>

class Game
{
public:
	static Game* Instance();
	void init();
	void reset();
	void restart();
	int hasWin();
	void updateCells(std::pair<int, int>, std::string);
	void createImage();
	void changeTurn();
	void addPlayer(std::string);

	inline std::string getTurnsList() { return mTurns; };
	inline std::pair<int, int> getLastCell() { return mLastCell; };
	inline std::string getPlayerTurn() { return mTurnPlayer; };
	inline int getWinner() { return mWinner; };
	inline bool hasWinner() { return mWinner != 0; };
	inline std::map<std::pair<int, int>, std::string> getCells() { return mCells; };
	inline std::map<std::pair<int, int>, sf::Shape*> getShapes() { return mCellShapes; };

private:
	Game();
	static Game* mInstance;

	//PlayerId for each game cell
	std::map<std::pair<int, int>, std::string> mCells;
	std::map<std::pair<int, int>, sf::Shape*> mCellShapes;
	std::vector<sf::Shape*> mShapes;
	sf::Texture* mTexture;
	int mWinner;
	std::string mTurnPlayer;
	std::vector<std::string> mPlayers;
	std::pair<int, int> mLastCell;
	std::string mTurns;
};