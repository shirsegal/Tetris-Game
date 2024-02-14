#pragma once
#include "shape.h"
#include "board.h"

class ComputerPlayer
{
public:
	enum { arrSIZE = 5 };
	enum eRotation { CLOCKWISE = 0, COUNTERCLOCKWISE };
	enum eSide { LEFT = -1, RIGHT = 1 };


private:
	Shape shape;
	Board myBoard;
	int index;
	int bestMoveScore;
	int bestMove[4];
	int score;
	void simulationMove();
	int evaluateMove()const;
	void copyArr(int from[arrSIZE], int moveScore);
	void updateMoveArr(int arr[arrSIZE]);
	void moveShapeDown(Shape& shape);
	int evaluateMove(Shape shape);
	void resetArr(int arr[arrSIZE]);
	int getTheLowestY(Shape& shape);
	int getTheHighestY(Shape& shape);
	int getMin(int& a, int b) const;
	bool checkAndRotate(bool& counterClock, int rotation, Shape& shape, int moveArr[arrSIZE]);
	bool moveAndEvaluateShapeSimulator(Shape& temp, int side, bool rotate, int moveArr[arrSIZE]);
	void keepRotating(int& i, int moveArr[arrSIZE], Shape& shape);
	void moveShapeOneDown(Shape& shape);

public:
	void generateMoves();

	void drawNewBoard(int playerIndex);
	void initPlayerBoard();
	void copyShapeToBoard();
	bool loseGame();
	void draw(int ch, int _x, int _y, int playerIndex);
	void copyShapeToBoard(Board& _board, Shape _shape);
	void checkIfThereIsFullLine(int playerIndex);
	int getTheLowestY();
	bool moveShape2();

	//player ctor
	ComputerPlayer(int i) : shape(myBoard) { index = i, this->initPlayerBoard(), score = 0; }

	void getRandShape(bool color)
	{
		shape.getRandShape(color);
		generateMoves();
	}

	int getScore()
	{
		return score;
	}

	int getIndex()
	{
		return index;
	}

	bool  moveShape(GameConfig::eKeys key)
	{
		return shape.move(key, index);
	}

	void drawShape(char ch = '#')
	{
		shape.drawShape(index, ch);
	}
};

