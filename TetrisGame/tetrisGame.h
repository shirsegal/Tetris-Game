#pragma once
#include "player.h"
#include "computerPlayer.h"

class TetrisGame
{
	bool gameRunning;
public:
	bool runGame();
	int startNewGame(bool color);
	int moveShape(ComputerPlayer& p1, ComputerPlayer& p2, bool color);
	bool loseGame(ComputerPlayer p);
	int noKeyPressed = 9;
	void finishGame(int score1, int score2, bool losePlayer1, bool losePlayer2);
};

