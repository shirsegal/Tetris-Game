#include "point.h"
#include "goToxy.h"
#include <Windows.h>

//function for the colors
void Point::draw(char ch, int& playerIndex, int backcolor)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), backcolor);
	int _x = x + (GameConfig::GAME_WIDTH + GameConfig::PLAYERS_SEPERATION + 2) * playerIndex;
	gotoxy(_x, y);
	cout << ch;
}


//Move points down or like the given key
void Point::move(GameConfig::eKeys key, bool& isOnSideL, bool& isOnSideR, bool& canLeft, bool& canRight)
{
	// Reset direction before processing a new movement
	diff_x = 0;
	diff_y = 1;

	switch (key)
	{
	case GameConfig::eKeys::LEFT:
		if (canLeft)
		{
			isOnSideR = false;
			diff_x = -1;
			diff_y = 0;
		}
		break;
	case GameConfig::eKeys::RIGHT:
		if (canRight)
		{
			isOnSideL = false;
			diff_x = 1;
			diff_y = 0;
		}
		break;
	}


	if (!isOnSideR && !isOnSideL)
	{
		x += diff_x;
		if (x > GameConfig::GAME_WIDTH)
			x = 1;
		else if (x == 0)
			x = GameConfig::GAME_WIDTH;
	}

	y += diff_y;
	if (y > GameConfig::GAME_HEIGHT + 1)
		y = 1;
	else if (y == 0)
		y = GameConfig::GAME_HEIGHT;
}