#pragma once
#include "player.h"
#include "computerPlayer.h"

class TetrisGame
{
	bool gameRunning = false;
	char chooseComputerLevel(int playerNum);
	Player* player1;
	Player* player2;
	void finishPlayerTurn(Player* player, int index, bool color);

public:
	enum eMenuKeys { START_HvsH = '1', START_HvsC = '2', START_CvsC = '3', CONTINUE = '4', INSTRUCTIONS = '8', EXIT = '9' };

	TetrisGame() : player1(nullptr), player2(nullptr) {}

	~TetrisGame() {
		deletePlayers();
	}

	void deletePlayers();
	bool runGame();
	int startNewGame(bool color);
	int moveShape(Player* player1, Player* player2, bool color);
	bool loseGame(ComputerPlayer p);
	void createNewPlayers(char p1, char p2);
	int noKeyPressed = 9;
	void finishGame(int score1, int score2, bool losePlayer1, bool losePlayer2);

	void changeGameStatus(bool res)
	{
		gameRunning = res;
	}

	bool getGameStatus()
	{
		return gameRunning;
	}

	bool checkIfPlayWithColor();
};

