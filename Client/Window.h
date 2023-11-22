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
	static Window* Instance();
	void update();
	void addShape(sf::Shape*);
	void addCell(std::pair<int, int>, sf::Shape*);
	void initCells(std::map<std::pair<int, int>, int>);

	inline std::map<std::pair<int, int>, struct cell> getCells() { return mCells; };
	inline int getTurn() { return mTurn; };
	inline bool isOpen() { return mWindow->isOpen(); };

	void Finito();
	void initTextFirstMenu();
	void initTextSecondMenu();
	void menuNameEnter();
	void changeMenuColor();
	void checkTextClick();

	enum SceneState {
		MAIN_MENU,
		NAME_MENU,
	};

	void changeScene(SceneState newState);

	SceneState getCurrentScene() const { return currentScene; }

private:
	Window();
	~Window();

	static Window* mInstance;

	int mTurn = 1;

	bool endGame = false;
	bool hasEnterName = true;

	sf::RenderWindow* mWindow;

	std::vector<sf::Shape*> mShapes;
	std::vector<sf::Text*> mTexts;
	std::vector<sf::Text*> mTextMenu;

	std::map<std::pair<int, int>, struct cell> mCells;

	sf::Font mFont;
	sf::Font mFontTitle;

	void checkCollision(sf::Event);
	void addPlayerShape(sf::Vector2f);
	int mTurn = 1;
	bool endGame = false;

	bool hasEnterName = true;
	int mWidth = 800;
	int mLength = 800;
	std::string mName;

	sf::Text mEnterName;

	sf::Event event;

	void checkCollision(sf::Event);
	void addPlayerShape(sf::Vector2f);

	SceneState currentScene;
};