#pragma once
#include "player.h"
#include "computerPlayer.h"

class TetrisGame
{
	bool gameRunning;
	void createNewPlayers(char p1, char p2, Player*& player1, Player*& player2);
	char chooseComputerLevel(int playerNum);
	//enum eComputerLevel { BEST = 'a', GOOD = 'b', NOVICE = 'c' };

public:
	bool runGame();
	int startNewGame(bool color, char p1, char p2);
	int moveShape(Player* player1, Player* player2, bool color);
	bool loseGame(ComputerPlayer p);
	int noKeyPressed = 9;
	void finishGame(int score1, int score2, bool losePlayer1, bool losePlayer2);
};

