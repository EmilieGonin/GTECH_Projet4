#include "Game.h"

Game* Game::mInstance = nullptr;

Game::Game()
{
	mWinner = 0;
	mTurnPlayer = 1;
	mTexture = new sf::Texture();
}

Game* Game::Instance()
{
	if (mInstance == nullptr) mInstance = new Game();
	return mInstance;
}

void Game::init()
{
	int cellNumber = 9;
	int cellSize = 150;
	int line = 0;
	int column = 0;

	for (size_t i = 1; i <= cellNumber; i++)
	{
		sf::RectangleShape* shape = new sf::RectangleShape(sf::Vector2f(cellSize, cellSize));
		shape->setPosition(sf::Vector2f(cellSize * line + 150, cellSize * column + 150));

		if (i % 2 == 0) shape->setFillColor(sf::Color::White);
		else shape->setFillColor(sf::Color::Blue);

		mCellShapes[{line, column}] = shape;
		mCells[{line, column}] = 0; //Init cells with no player
		line++;
		if (i % 3 == 0) line = 0, column++;
	}
}

void Game::reset()
{
	delete mTexture;
	for (auto& shape : mShapes) delete shape;
	for (auto& cell : mCellShapes) delete cell.second;
	delete mInstance;
	mInstance = NULL;
}

bool Game::hasWin()
{
	for (size_t i = 0; i < 3; i++)
	{
		//Columns
		if (mCells[{i, 0}] == mTurnPlayer && mCells[{i, 1}] == mTurnPlayer && mCells[{i, 2}] == mTurnPlayer) return true;

		//Lines
		if (mCells[{0, i}] == mTurnPlayer && mCells[{1, i}] == mTurnPlayer && mCells[{2, i}] == mTurnPlayer) return true;

		//Diagonales
		if (mCells[{0, 0}] == mTurnPlayer && mCells[{1, 1}] == mTurnPlayer && mCells[{2, 2}] == mTurnPlayer) return true;
		if (mCells[{0, 2}] == mTurnPlayer && mCells[{1, 1}] == mTurnPlayer && mCells[{2, 0}] == mTurnPlayer) return true;
	}

	return false;
}

void Game::updateCells(std::pair<int, int> cell, int player)
{
	mTurnPlayer = player;
	mCells[{cell.first, cell.second}] = player;

	sf::CircleShape* shape = new sf::CircleShape(75);
	shape->setPosition(mCellShapes[{cell.first, cell.second}]->getPosition());

	if (mTurnPlayer == 1) shape->setFillColor(sf::Color::Green);
	else shape->setFillColor(sf::Color::Red);

	mShapes.push_back(shape);
	changeTurn();
}

void Game::createImage()
{
	sf::RenderWindow window(sf::VideoMode(800, 800), "Tic-tac-toe");
	for (auto& shape : mShapes) window.draw(*shape);
	for (auto& cell : mCellShapes) window.draw(*cell.second);

	mTexture->create(800, 800);
	mTexture->update(window);

	sf::Image image = mTexture->copyToImage();
	image.saveToFile("game.png");
}

void Game::changeTurn()
{
	mTurnPlayer = mTurnPlayer == 1 ? 2 : 1;
}