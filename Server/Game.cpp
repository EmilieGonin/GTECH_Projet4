#include "Game.h"

Game* Game::mInstance = nullptr;

Game::Game()
{
	mWinner = 0;
	mTurnPlayer = "";
	mTurns = "";
	mTexture = new sf::Texture();
	mLastCell = { -1, -1 };
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
		mCells[{line, column}] = ""; //Init cells with no player
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

void Game::restart()
{
	mShapes.clear();
	mCells.clear();
	mCellShapes.clear();
	mWinner = 0;
	mTurnPlayer = "";
	mTurns = "";
	mTexture = new sf::Texture();
	mLastCell = { -1, -1 };
	mPlayers.clear();
}

int Game::hasWin()
{
	std::string previousPlayer = mTurnPlayer == mPlayers[0] ? mPlayers[1] : mPlayers[0];

	for (size_t i = 0; i < 3; i++)
	{
		//Columns
		if (mCells[{i, 0}] == previousPlayer && mCells[{i, 1}] == previousPlayer && mCells[{i, 2}] == previousPlayer) return 1;

		//Lines
		if (mCells[{0, i}] == previousPlayer && mCells[{1, i}] == previousPlayer && mCells[{2, i}] == previousPlayer) return 1;

		//Diagonales
		if (mCells[{0, 0}] == previousPlayer && mCells[{1, 1}] == previousPlayer && mCells[{2, 2}] == previousPlayer) return 1;
		if (mCells[{0, 2}] == previousPlayer && mCells[{1, 1}] == previousPlayer && mCells[{2, 0}] == previousPlayer) return 1;
	}

	if (mShapes.size() == 9) return 0;

	return -1;
}

void Game::updateCells(std::pair<int, int> cell, std::string player)
{
	mLastCell = cell;
	mTurnPlayer = player;
	mCells[{cell.first, cell.second}] = player;

	sf::CircleShape* shape = new sf::CircleShape(75);
	shape->setPosition(mCellShapes[{cell.first, cell.second}]->getPosition());

	if (mTurnPlayer == mPlayers[0]) shape->setFillColor(sf::Color::Green);
	else shape->setFillColor(sf::Color::Red);

	mShapes.push_back(shape);

	mTurns += mCells[cell] + " played " + std::to_string(cell.first) + ":" + std::to_string(cell.second) + "<br />";

	changeTurn();
	createImage();
}

void Game::createImage()
{
	sf::RenderWindow window(sf::VideoMode(800, 800), "Tic-tac-toe");
	for (auto& cell : mCellShapes) window.draw(*cell.second);
	for (auto& shape : mShapes) window.draw(*shape);

	mTexture->create(800, 800);
	mTexture->update(window);

	sf::Image image = mTexture->copyToImage();
	image.saveToFile("game.png");
}

void Game::changeTurn()
{
	mTurnPlayer = mTurnPlayer == mPlayers[0] ? mPlayers[1] : mPlayers[0];
}

void Game::addPlayer(std::string playerId)
{
	if (mTurnPlayer.empty()) mTurnPlayer = playerId;
	mPlayers.push_back(playerId);
}