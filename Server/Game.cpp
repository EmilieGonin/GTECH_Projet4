#include "Game.h"

Game* Game::mInstance = nullptr;

Game::Game()
{
	mWinner = 0;
	mTurnPlayer = 1;
}

Game* Game::Instance()
{
	if (mInstance == nullptr)
	{
		mInstance = new Game();
	}

	return mInstance;
}

void Game::init()
{
	int cellNumber = 9;
	int line = 0;
	int column = 0;

	for (size_t i = 1; i <= cellNumber; i++)
	{
		mCells[{line, column}] = 0; //Init cells with no player
		line++;
		if (i % 3 == 0) line = 0, column++;
	}
}

void Game::update()
{
	//
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
	mCells[{cell.first, cell.second}] = player;
}