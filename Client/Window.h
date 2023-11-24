#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window/Mouse.hpp>
#include <map>
#include <string>
#include <iostream>
#include "Client.h"

struct cell {
	sf::Shape* shape;
	std::pair<int, int> pos;
	std::string player;
};

enum SceneState {
	MAIN_MENU,
	WAITING,
	SKINS,
	GAME,
	END_GAME,
};

class Window
{
public:
	static Window* Instance(Client* client);
	int update();
	void addShape(sf::Shape*);
	void addCell(std::pair<int, int>, sf::Shape*);
	void initCells(std::map<std::pair<int, int>, std::string>);
	std::pair<int, int> play();
	void resetTurn(bool);
	void initTurnsList(std::map<std::pair<int, int>, std::string>, std::pair<int, int>);
	void restart();

	inline void setWinner(std::string winnerId) { mWinner = winnerId; };
	inline void setPlayer(std::string playerId) { mPlayerId = playerId; };
	inline std::string getPlayer() { return mPlayerId; };
	inline bool hasPlayed() { return mHasPlayed; };
	inline bool hasSelectedCell() { return mSelectedCell.first != -1; };
	inline std::map<std::pair<int, int>, struct cell> getCells() { return mCells; };
	inline int getTurn() { return mTurn; };
	inline bool isOpen() { return mWindow->isOpen(); };

	void menuNameEnter();
	void changeMenuColor();
	int checkTextClick();

	void textMainMenu();
	void shapeMainMenu();
	void screenEndGame();
	void waitingScreen();
	void skinsScreen();

	void shapesWindow();
	void colorsWindow();

	void changeScene(SceneState newState);

	SceneState getCurrentScene() const { return currentScene; }

private:
	Window();
	~Window();

	static Window* mInstance;

	int mTurn = 1;
	int cote = 175.f;

	const unsigned int maxNameLength = 10;

	bool endGame = false;

	sf::RenderWindow* mWindow;

	std::vector<sf::Shape*> mShapes;
	std::vector<sf::Text*> mTexts;
	std::vector<sf::Text*> mTextMenu;
	std::vector<sf::RectangleShape*> mButton;

	std::map<std::pair<int, int>, cell> mCells;

	sf::Font mFont;
	sf::Font mFontTitle;

	void checkCollision(sf::Event);
	void addPlayerShape(sf::Vector2f, std::string);

	int mWidth = 800;
	int mLength = 800;
	std::string mName;

	sf::Text* mEnterName;
	sf::Text* mErrorMessage;
	bool hasError;

	sf::Event event;

	SceneState currentScene;

	std::pair<int, int> mSelectedCell;
	bool mHasPlayed;
	std::string mPlayerId;
	std::string mWinner;
	std::string mTurns;

	Client* client;

	std::map<std::pair<int, int>, sf::Shape*> mSkinsCells;
	std::map<std::pair<int, int>, sf::Shape*> skins;

};