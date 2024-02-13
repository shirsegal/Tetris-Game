#pragma once
#include "player.h"
class TetrisGame
{
	bool gameRunning;
public:
	bool runGame();
	int startNewGame(bool color);
	int moveShape(Player& p1, Player& p2, bool color);
	bool loseGame(Player p);
	int noKeyPressed = 9;
	void finishGame(int score1, int score2, bool losePlayer1, bool losePlayer2);
};

