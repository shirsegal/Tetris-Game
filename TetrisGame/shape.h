#pragma once
#include "Point.h"
#include "board.h"


class Shape
{
public:
	enum { SIZE = 4 };
	enum eShapes { squereShape = 1, LRightShape, LLeftShape, TShape, ZLeftfShape, ZRightfShape, columShape, Bomb };

private:
	enum startVal { X = 6, Y = 2 };
	Point body[SIZE];
	Board& myBoard;
	Point bomb;
	bool isBomb = false;
	int noKeyPressed = 9;
	enum class eDistance { FORx = 1, FORy = 2 };
	enum ePlaces { FIRST = 0, SECOND, THIRD, FOURTH };
	enum class eColors { BLACK, RED, BLUE, GREEN, PURPLE, YELLOW };
	enum class eDirection { HORIZON = 1, VERTICAL, UPSIDE_DOWN, UPSIDE_LEFT };
	int direction;
	void dropShape();
	bool isOnSideLeft();
	bool isOnSideRight();
	int color;
	int shapeNum;

public:
	Shape(Board& board) : myBoard(board) {};
	void getRandShape(bool color);
	void dupShape(Point from[SIZE], Point to[SIZE]);
	void drawShape(int playerIndex, char ch = '#');
	bool move(GameConfig::eKeys key, int playerIndex);
	void setShape(int shapeNum, int shapeColor);
	bool rotateClockwise(int playerIndex);
	bool canRotate();
	bool rotateCounterClockwise(int playerIndex);
	bool canMoveRight(); 
	bool canMoveLeft(); 
	bool reachExistingShape(); 
	bool inBottom();
	int getLeftmostEdge();

	int getDirection()
	{
		return direction;
	}

	void setDirection(Shape::eDirection _direction)
	{
		direction = (int)_direction;
	}

	Point& getBodyPoint(int i)
	{
		return body[i];
	}

	bool itsBomb() const
	{
		return isBomb;
	}

	int getColor() const
	{
		return color;
	}

	int getShapeNum()const
	{
		return shapeNum;
	}

	Point& getBomb()
	{
		return bomb;
	}

	void setBomb(int x, int y)
	{
		bomb.setXY(x, y);
	}
};

