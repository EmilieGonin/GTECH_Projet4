#pragma once
#include <SFML/Graphics.hpp>
#include <map>

class Game
{
public:
	static Game* Instance();
	void init();
	void reset();
	bool hasWin();
	void updateCells(std::pair<int, int>, int);
	void createImage();
	void changeTurn();

	inline int getPlayerTurn() { return mTurnPlayer; };
	inline int getWinner() { return mWinner; };
	inline bool hasWinner() { return mWinner != 0; };
	inline std::map<std::pair<int, int>, int> getCells() { return mCells; };
	inline std::map<std::pair<int, int>, sf::Shape*> getShapes() { return mCellShapes; };

private:
	Game();
	static Game* mInstance;

	//PlayerId for each game cell
	std::map<std::pair<int, int>, int> mCells;
	std::map<std::pair<int, int>, sf::Shape*> mCellShapes;
	std::vector<sf::Shape*> mShapes;
	sf::Texture* mTexture;
	int mWinner;
	int mTurnPlayer;
};