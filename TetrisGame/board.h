#pragma once
#include "gameConfig.h"
#include "goToxy.h"

class Board
{
	int gameBoard[GameConfig::GAME_HEIGHT][GameConfig::GAME_WIDTH];
	void initBoard(int board[GameConfig::GAME_HEIGHT][GameConfig::GAME_WIDTH]);
	void copyNewBoard(int board[GameConfig::GAME_HEIGHT][GameConfig::GAME_WIDTH], int y);


public:
	void drawBoards();
	void drawBorder(int x, int y);
	void initPlayerBoard();
	bool loseGame();
	int checkIfThereIsFullLine(int _numOfRows);
	int countHoles(int y, int x) const;
	int calculateBoardSurface() const;
	int getColumnHeight(int x) const;


	int getBoardYX(int y, int x)
	{
		return gameBoard[y][x];
	}

	void setBoardYX(int y, int x, int set)
	{
		gameBoard[y][x] = set;
	}

};