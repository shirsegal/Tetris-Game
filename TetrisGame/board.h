#pragma once
#include "gameConfig.h"
#include "goToxy.h"
#include "point.h"

class Board
{
	Point bomb;
	int gameBoard[GameConfig::GAME_HEIGHT][GameConfig::GAME_WIDTH];
	void initBoard(int board[GameConfig::GAME_HEIGHT][GameConfig::GAME_WIDTH]) const;
	void copyNewBoard(int board[GameConfig::GAME_HEIGHT][GameConfig::GAME_WIDTH], int y);
	int checkMin(int a, int b) const;
	int checkMax(int a, int b) const;


public:
	void drawBoards();
	void drawBorder(int x, int y);
	void initPlayerBoard();
	bool loseGame() const;
	int checkIfThereIsFullLine(int _numOfRows);
	int countHoles(int y, int x) const;
	int calculateBoardSurface() const;
	int getColumnHeight(int x) const;
	void handleBombExploade();


	int getBoardYX(int y, int x)
	{
		return gameBoard[y][x];
	}

	void setBoardYX(int y, int x, int set)
	{
		gameBoard[y][x] = set;
	}

	void setBomb(int x, int y)
	{
		bomb.setXY(x, y);
	}

};