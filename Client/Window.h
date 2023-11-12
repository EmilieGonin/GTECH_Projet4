#pragma once
#include <SFML/Graphics.hpp>
#include <map>

class Window
{
public:
	Window();
	void update();
	void addShape(sf::Shape*);
	void addCell(sf::Shape*);

	inline bool isOpen() { return mWindow->isOpen(); };

private:
	sf::RenderWindow* mWindow;
	std::vector<sf::Shape*> mShapes;
	std::map<sf::Shape*, bool> mCells;
	int mTurn = 1;

	void checkCollision(sf::Event);
	void addPlayer(sf::Vector2f);
};