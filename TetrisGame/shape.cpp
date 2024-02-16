#include "shape.h"
#include <stdlib.h> 
#include <time.h> 
#include <Windows.h> 
#include <iostream>

//The function is menage what shape on board now
void Shape::setShape(int shapeNum, int shapeColor)
{
	switch (shapeColor)
	{
	case (int)eColors::BLACK:
		color = FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN;
		break;
	case (int)eColors::RED:
		color = FOREGROUND_RED;
		break;
	case (int)eColors::BLUE:
		color = FOREGROUND_BLUE;
		break;
	case (int)eColors::GREEN:
		color = FOREGROUND_GREEN;
		break;
	case (int)eColors::PURPLE:
		color = FOREGROUND_RED | FOREGROUND_BLUE;
		break;
	case (int)eColors::YELLOW:
		color = FOREGROUND_RED | FOREGROUND_GREEN;
		break;
	}

	switch (shapeNum)
	{
	case (int)eShapes::columShape:
	{
		body[(int)ePlaces::FIRST].setXY(6, 2);
		body[(int)ePlaces::SECOND].setXY(6, 3);
		body[(int)ePlaces::THIRD].setXY(6, 4);
		body[(int)ePlaces::FOURTH].setXY(6, 5);
	}
	break;
	case (int)eShapes::LLeftShape:
	{
		body[(int)ePlaces::FIRST].setXY(7, 2);
		body[(int)ePlaces::SECOND].setXY(7, 3);
		body[(int)ePlaces::THIRD].setXY(7, 4);
		body[(int)ePlaces::FOURTH].setXY(8, 4);
	}
	break;
	case (int)eShapes::LRightShape:
	{
		body[(int)ePlaces::FIRST].setXY(7, 2);
		body[(int)ePlaces::SECOND].setXY(7, 3);
		body[(int)ePlaces::THIRD].setXY(7, 4);
		body[(int)ePlaces::FOURTH].setXY(6, 4);
	}
	break;
	case (int)eShapes::squereShape:
	{
		body[(int)ePlaces::FIRST].setXY(6, 2);
		body[(int)ePlaces::SECOND].setXY(7, 2);
		body[(int)ePlaces::THIRD].setXY(6, 3);
		body[(int)ePlaces::FOURTH].setXY(7, 3);
	}
	break;
	case (int)eShapes::TShape:
	{
		setDirection(eDirection::VERTICAL);
		body[(int)ePlaces::FIRST].setXY(5, 2);
		body[(int)ePlaces::SECOND].setXY(6, 2);
		body[(int)ePlaces::THIRD].setXY(7, 2);
		body[(int)ePlaces::FOURTH].setXY(6, 3);
	}
	break;
	case (int)eShapes::ZLeftfShape:
	{
		body[(int)ePlaces::FIRST].setXY(6, 2);
		body[(int)ePlaces::SECOND].setXY(7, 2);
		body[(int)ePlaces::THIRD].setXY(7, 3);
		body[(int)ePlaces::FOURTH].setXY(8, 3);
	}
	break;
	case (int)eShapes::ZRightfShape:
	{
		body[(int)ePlaces::FIRST].setXY(7, 2);
		body[(int)ePlaces::SECOND].setXY(8, 2);
		body[(int)ePlaces::THIRD].setXY(7, 3);
		body[(int)ePlaces::FOURTH].setXY(6, 3);
	}
	break;
	case (int)eShapes::bomb:
	{
		isBomb = true;
		bomb.setPoint(6, 2, '@');
	}
	break;
	}

}

//The function return the x's shape that close to left border
int Shape::getLeftmostEdge()
{
	int leftmost = GameConfig::GAME_WIDTH;

	for (int i = 0; i < SIZE; ++i)
	{
		int x = body[i].getX();
		if (x < leftmost)
			leftmost = x;
	}

	return leftmost;
}

void Shape::getRandShape(bool color)
{
	int sNum = rand() % 8 + 1; //without bomb 7
	int colorNum = (int)eColors::BLACK;

	if (color)
		colorNum = rand() % 5 + 1;

	setShape(sNum, colorNum);
	shapeNum = sNum;
}

//The function rotate the shape counter clockwise
bool Shape::rotateCounterClockwise(int playerIndex)
{
	int getX, getY;
	Point tmp[SIZE];
	dupShape(body, tmp);


	for (Point& p : body)
	{
		getX = p.getX() - tmp[2].getX();
		getY = p.getY() - tmp[2].getY();
		p.setPoint(tmp[2].getX() + getY, tmp[2].getY() - getX);
	}

	if (canRotate())
	{
		for (int i = 0; i < Shape::SIZE; i++)
			tmp[i].draw(' ', playerIndex, color);
		return true;
	}
	else
	{
		for (int i = 0; i < Shape::SIZE; i++)
			body[i] = tmp[i];
		return false;
	}
}

//The function rotate the shape clockwise
bool Shape::rotateClockwise(int playerIndex)
{
	int getX, getY;
	Point tmp[SIZE];
	dupShape(body, tmp);


	for (Point& p : body)
	{
		getX = p.getX() - tmp[2].getX();
		getY = p.getY() - tmp[2].getY();
		p.setPoint(tmp[2].getX() - getY, tmp[2].getY() + getX);
	}

	if (canRotate())
	{
		for (int i = 0; i < Shape::SIZE; i++)
			tmp[i].draw(' ', playerIndex, color);
		return true;
	}
	else
	{
		for (int i = 0; i < Shape::SIZE; i++)
			body[i] = tmp[i];
		return false;
	}
}

//The function check if can do the rotate before it change
bool Shape::canRotate()
{

	for (Point& p : body)
	{
		int x = p.getX() - (int)eDistance::FORx;
		int y = p.getY() - (int)eDistance::FORy;

		//gameBoard[y][x] != ' ' בורד קודם
		if (myBoard.getBoardYX(y, x) != ' ' || x <= (int)eDistance::FORx ||
			x >= GameConfig::GAME_WIDTH || y >= (GameConfig::GAME_HEIGHT + (int)eDistance::FORx))
		{
			return false;
		}
	}
}

//duplicate the shape
void Shape::dupShape(Point from[SIZE], Point to[SIZE])
{
	for (int i = 0; i < SIZE; i++)
		to[i].setPoint(from[i].getX(), from[i].getY());
}

//The function draw current shape
void Shape::drawShape(int playerIndex, char ch)
{
	for (int i = 0; i < SIZE; i++)
		body[i].draw(ch, playerIndex, color);
}

//check if the shape touch an existing shape in the board
bool Shape::reachExistingShape()
{
	if (isBomb)
	{
		int x = bomb.getX() - (int)eDistance::FORx;
		int y = bomb.getY() - (int)eDistance::FORy;
		if (myBoard.getBoardYX(y + (int)eDistance::FORx, x) != ' ')
			return true;
	}
	else
	{
		for (int i = 0; i < Shape::SIZE; i++)
		{
			int x = body[i].getX() - (int)eDistance::FORx;
			int y = body[i].getY() - (int)eDistance::FORy;
			if (myBoard.getBoardYX(y + (int)eDistance::FORx, x) != ' ')
				return true;
		}
	}
	return false;
}

//The function is handle the shape moves
bool Shape::move(GameConfig::eKeys key, int playerIndex)
{
	bool onSideLeft, onSideRight, canRight = true, canLeft = true, touchShape;
	if (key == GameConfig::eKeys::ROTATE_CLOCKWISE && isBomb == false)
		rotateClockwise(playerIndex);

	else if (key == GameConfig::eKeys::ROTATE_COUNTER_CLOCKWISE && isBomb == false)
		rotateCounterClockwise(playerIndex);

	else if (key == GameConfig::eKeys::DROP)
		dropShape();

	if (key == GameConfig::eKeys::LEFT)
	{
		canLeft = canMoveLeft();
	}
	else if (key == GameConfig::eKeys::RIGHT)
	{
		canRight = canMoveRight();
	}

	touchShape = reachExistingShape();

	//check if the shape can go down 
	if (!inBottom() && !touchShape)
	{
		onSideLeft = isOnSideLeft();
		onSideRight = isOnSideRight();


		for (int i = 0; i < SIZE; i++)
		{
			if (isBomb)
				bomb.move(key, onSideLeft, onSideRight, canLeft, canRight);
			else
				body[i].move(key, onSideLeft, onSideRight, canLeft, canRight);
		}
	}
	else
	{
		if (isBomb)
			myBoard.setBomb(bomb.getX(), bomb.getY());
		else
			drawShape(playerIndex);
		return false;
	}
}

//The function check if the shape can move left
bool Shape::canMoveLeft()
{
	if (isBomb)
	{
		if (myBoard.getBoardYX(bomb.getY() - (int)eDistance::FORy, bomb.getX() - (int)eDistance::FORy) != ' ')
		{
			return false;
		}
	}
	else
	{
		for (int i = 0; i < Shape::SIZE; i++)
		{
			if (myBoard.getBoardYX(body[i].getY() - (int)eDistance::FORy, body[i].getX() - (int)eDistance::FORy) != ' ')
			{
				return false;
			}
		}
	}
	return true;
}

//The function check if the shape can move right
bool Shape::canMoveRight()
{
	if (isBomb)
	{
		if (myBoard.getBoardYX(bomb.getY() - (int)eDistance::FORy, bomb.getX()) != ' ')
		{
			return false;
		}
	}
	else
	{
		for (int i = 0; i < Shape::SIZE; i++)
		{
			if (myBoard.getBoardYX(body[i].getY() - (int)eDistance::FORy, body[i].getX()) != ' ')
			{
				return false;
			}
		}
	}
	return true;
}

void Shape::dropShape()
{
	if (isBomb)
	{
		Point tempB;
		int _y = bomb.getY() + 1;
		if (_y >= GameConfig::GAME_HEIGHT + (int)eDistance::FORy)
			return;
		else if (reachExistingShape())
			return;

		int _x = bomb.getX();
		tempB.setPoint(_x, _y, '@');
		bomb = tempB;
	}
	else
	{
		Point temp[SIZE];
		dupShape(body, temp);

		for (int i = 0; i < SIZE; i++)
		{
			int _y = body[i].getY() + 1;
			if (_y >= GameConfig::GAME_HEIGHT + (int)eDistance::FORy)
				return;
			else if (reachExistingShape())
				return;

			int _x = body[i].getX();
			temp[i].setPoint(_x, _y);
		}
		dupShape(temp, body);
	}

}

//Check if the shape is in the bottom
bool Shape::inBottom()
{
	if (isBomb)
	{
		if (bomb.getY() >= (GameConfig::GAME_HEIGHT + (int)eDistance::FORx))
			return true;
	}
	else
	{
		for (Point& p : body)
		{
			if (p.getY() >= (GameConfig::GAME_HEIGHT + (int)eDistance::FORx))
				return true;
		}
	}
	return false;
}

//Check if the shape in on the side of the borde
bool Shape::isOnSideLeft()
{
	for (Point& p : body)
	{
		if (p.getX() <= (int)eDistance::FORx)
			return true;
	}
	return false;
}

//Check if the shape in on the side of the borde
bool Shape::isOnSideRight()
{
	for (Point& p : body)
	{
		if (p.getX() >= GameConfig::GAME_WIDTH)
			return true;
	}
	return false;
}