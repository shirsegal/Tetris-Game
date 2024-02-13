#pragma once
#include "Board.h"
#include "point.h"
#include "shape.h"
#include "gameConfig.h"
#include "goToxy.h"


class Player
{
	int score;
	int index;
	Board myBoard;
	Shape shape;
	
	int getTheLowestY();
	void copyNewBoard(int board[GameConfig::GAME_HEIGHT][GameConfig::GAME_WIDTH], int y);
	void initBoard(int board[GameConfig::GAME_HEIGHT][GameConfig::GAME_WIDTH]);
	void draw(int ch, int _x, int _y,int playerIndex);

public:

	void drawNewBoard(int playerIndex);
	void initPlayerBoard();
	void copyShapeToBoard();
	bool loseGame();
	void checkIfThereIsFullLine(int playerIndex);

	//player ctor
	Player(int i): shape(myBoard) { index = i, this->initPlayerBoard(), score = 0; }

	Shape getShape()
	{
		return shape;
	}

	int getIndex()
	{
		return index;
	}

	int getScore()
	{
		return score;
	}

	void drawShape(char ch = '#')
	{
		shape.drawShape(index, ch);
	}

	bool  moveShape(GameConfig::eKeys key)
	{
		return shape.move(key, index);
	}

	void getRandShape(bool color)
	{
		shape.getRandShape(color);
	}
};

