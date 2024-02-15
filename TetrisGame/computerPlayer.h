#pragma once
#include "shape.h"
#include "board.h"
#include "player.h"

class ComputerPlayer : public Player
{
public:
	enum { arrSIZE = 5 };
	enum eRotation { CLOCKWISE = 0, COUNTERCLOCKWISE };
	enum eSide { LEFT = -1, RIGHT = 1 };


private:
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

	bool moveShape(GameConfig::eKeys key) override
	{
		return moveShape2(); //TODOOOOO!!!!!!!
	}

public:
	void generateMoves();

	void copyShapeToBoard(Board& _board, Shape _shape);
	bool moveShape2();

	//player ctor
	ComputerPlayer(int i) : Player(i) { }

	void getRandShape(bool color) override
	{
		shape.getRandShape(color);
		generateMoves();
	}

	~ComputerPlayer() {}

};

