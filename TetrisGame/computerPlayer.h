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
	void generateMoves();
	void simulationMove();
	int evaluateMove()const;
	void copyAndResetArr(int from[arrSIZE], int moveScore);
	void updateMoveArr(int arr[arrSIZE]);
	void moveShapeDown(Shape& shape);
	int evaluateMove(Shape shape);
	void resetArr(int arr[arrSIZE]);
	int getTheLowestY(Shape& shape);
	int getTheHighestY(Shape& shape);
	int getMin(int& a, int b) const;
	bool checkAndRotate(bool& counterClock, int rotation, Shape shape, int moveArr[arrSIZE]);
	void moveAndEvaluateShapeSimulator(Shape temp, int side, bool rotate, int moveArr[arrSIZE]);
	void keepRotating(int& i, int moveArr[arrSIZE], Shape shape);
	void moveShapeOneDown(Shape shape);
};

