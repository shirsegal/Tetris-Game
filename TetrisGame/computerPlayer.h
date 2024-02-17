#pragma once
#include "shape.h"
#include "board.h"
#include "player.h"

class ComputerPlayer : public Player
{
public:
	enum { arrSIZE = 5 };
	enum eRotation { CLOCKWISE = 0, COUNTERCLOCKWISE };
	enum eSide { LEFT = 0, RIGHT = 1 };
	enum eLevel { BEST = 'a', GOOD = 'b', NOVICE = 'c' };

private:
	int bestMoveScore;
	int bestMove[4];
	int score;
	int noKeyPressed = 9;
	char level;

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
	bool moveAndEvaluateShapeSimulator(Shape& temp, int side, bool rotate, int moveArr[arrSIZE], bool chooseRand);
	void keepRotating(int& i, int moveArr[arrSIZE], Shape& shape);
	void moveShapeOneDown(Shape& shape);
	bool moveShape(GameConfig::eKeys key) override;
	bool canMoveLeftAndDown(Shape& shape, bool down) const;
	bool canMoveRightAndDown(Shape& shape, bool down) const;
	void bringShapeToStartPosition(Shape& temp, Shape& shape);

public:
	void generateMoves();

	void copyShapeToBoard(Board& _board, Shape _shape);

	//ComputerPlayer ctor
	ComputerPlayer(int i, char _level) : Player(i) { level = _level; }

	void getRandShape(bool color) override
	{
		shape.getRandShape(color);
		generateMoves();
	}

	~ComputerPlayer() {}

};

