#ifndef __GAME_CONFIG_H
#define __GAME_CONFIG_H

#include <string>
#include <Windows.h>

class GameConfig
{
public:
	enum class eKeys { LEFT, RIGHT, ROTATE_CLOCKWISE, ROTATE_COUNTER_CLOCKWISE, DROP };
	std::string p1Keys = "adswxADSWX";
	std::string p2Keys = "jlkimJLKIM";

	static constexpr int numOfMoves = 5;
	static constexpr int numOfKeys = 10;//For capital letter

	static constexpr int GAME_WIDTH = 12;
	static constexpr int GAME_HEIGHT = 18;
	static constexpr int PLAYERS_SEPERATION = 4;

	static constexpr int MIN_X = 1;
	static constexpr int MIN_Y = 2;
	static constexpr int BLACK_COLOR = FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN;

	//Return the font to black 
	void setBlackFont()
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), GameConfig::BLACK_COLOR);
	}
};
#endif