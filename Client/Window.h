#pragma once
#include <SFML/Graphics.hpp>

class Window
{
public:
	Window();
	void update();
	void addShape(sf::Shape*);

	inline bool isOpen() { return mWindow->isOpen(); };

private:
	sf::RenderWindow* mWindow;
	std::vector<sf::Shape*> mShapes;
};