#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window/Mouse.hpp>
#include <map>
#include <string>

struct cell {
	sf::Shape* shape;
	int player;
};

class Window
{
public:
	Window();
	~Window();
	void update();
	void addShape(sf::Shape*);
	void addCell(std::pair<int, int>, sf::Shape*);

	inline std::map<std::pair<int, int>, struct cell> getCells() { return mCells; };
	inline int getTurn() { return mTurn; };
	inline bool isOpen() { return mWindow->isOpen(); };
	void Finito();
	void initTextFirstMenu();
	void initTextSecondMenu();
private:
	sf::RenderWindow* mWindow;
	std::vector<sf::Shape*> mShapes;
	std::vector<sf::Text*> mTexts;
	std::map<std::pair<int, int>, struct cell> mCells;
	sf::Font mFont;
	sf::Font mFontTitle;

	void checkCollision(sf::Event);
	void addPlayerShape(sf::Vector2f);
	int mTurn = 1;
	bool endGame = false;

	bool hasEnterName = true;
	std::string mName;
	sf::Text mEnterName;
};