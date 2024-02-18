#pragma once
#include "gameConfig.h"


class Point
{
	int x;
	int y;
	char ch;
	int diff_x = 0;
	int diff_y = 0;

public:

	Point(int _x, int _y, char _ch = '#') : x(_x), y(_y), ch(_ch) {};

	Point() {}; // empty constructor

	void draw(char ch, int& playerIndex, int backcolor);
	void move(GameConfig::eKeys key, bool& isOnSideL, bool& isOnSideR, bool& canLeft, bool& canRight);

	void setPoint(int _x, int _y, char _ch = '#')
	{
		x = _x;
		y = _y;
		ch = _ch;
	}

	int getX() const
	{
		return x;
	}

	int getY() const
	{
		return y;
	}

	char getChar() const
	{
		return ch;
	}

	void setXY(int _x, int _y)
	{
		x = _x;
		y = _y;
	}
};