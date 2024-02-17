#include "tetrisGame.h"
#include "Board.h"
#include "menu.h"
#include "player.h"
#include "shape.h"
#include "gameConfig.h"
#include <conio.h> // for kbhit+getch
#include <iostream>
#include <Windows.h> 
#include "humanPlayer.h"
#include "computerPlayer.h"

using namespace std;

void TetrisGame::deletePlayers()
{
	if (player1 != nullptr) {
		delete player1;
		player1 = nullptr;
	}

	if (player2 != nullptr) {
		delete player2;
		player2 = nullptr;
	}
}

bool TetrisGame::checkIfPlayWithColor()
{
	cout << "Do you want to play with colors?" << endl;
	cout << "(1) Yes" << endl;
	cout << "(2) No" << endl;

	char playerChoice = _getch();
	while (playerChoice != '1' && playerChoice != '2')
		playerChoice = _getch();

	clrscr();
	if (playerChoice == '1')return true;
	else return false;
}

bool TetrisGame::runGame()
{
	Menu menu;
	bool gamePaused = false;
	int playerChoice = menu.goToMenu(gamePaused);

	while (playerChoice != EXIT)
	{
		if (gameRunning)
		{
			deletePlayers();
			changeGameStatus(false);
		}

		switch (playerChoice)
		{
		case (int)START_HvsH:
			clrscr();
			createNewPlayers('h', 'h');
			playerChoice = startNewGame(checkIfPlayWithColor());//Start a game human VS human
			break;
		case (int)START_HvsC:
			clrscr();
			createNewPlayers('h', 'c');
			playerChoice = startNewGame(checkIfPlayWithColor());//Start a game human VS computer
			break;
		case (int)START_CvsC:
			clrscr();
			createNewPlayers('c', 'c');
			playerChoice = startNewGame(checkIfPlayWithColor());//Start a game computer VS computer
			break;
		case (int)INSTRUCTIONS:
			playerChoice = menu.goToMenu(gamePaused);
			break;
		}
	}
	return 0;
}

void TetrisGame::createNewPlayers(char p1, char p2)
{
	int level;
	if (p1 == 'h') {
		player1 = new HumanPlayer(0);
	}
	else {
		level = chooseComputerLevel(1);
		player1 = new ComputerPlayer(0, level);
	}
	if (p2 == 'h') {
		player2 = new HumanPlayer(1);
	}
	else {
		level = chooseComputerLevel(2);
		player2 = new ComputerPlayer(1, level);
	}
}

int TetrisGame::startNewGame(bool color)
{
	Board board;
	changeGameStatus(true);
	board.drawBoards();

	bool losePlayer1 = false;
	bool losePlayer2 = false;

	player1->getRandShape(color);
	player2->getRandShape(color);

	// Continue the game until we have a winner or someone stops the game
	while (!losePlayer1 && !losePlayer2)
	{
		int res = moveShape(player1, player2, color);
		if (res != 0)
			return res;

		losePlayer1 = player1->loseGame();
		losePlayer2 = player2->loseGame();
	}

	// Determine the winner
	if (losePlayer1 || losePlayer2)
		finishGame(player1->getScore(), player2->getScore(), losePlayer1, losePlayer2);
}

int TetrisGame::moveShape(Player* player1, Player* player2, bool color)
{
	GameConfig ch;
	GameConfig key;

	int player1Index = player1->getIndex();
	int player2Index = player2->getIndex();
	int keyPressed = noKeyPressed;
	bool canRight2 = true, canLeft2 = true, canRight1 = true, canLeft1 = true, moveShape2 = true, moveShape1 = true;

	if (_kbhit())
	{
		keyPressed = _getch();
		if (keyPressed == 27)//Open the main menu
		{
			Menu menu;

			int res = menu.goToMenu(true);
			while (res == Menu::eMenuKeys::INSTRUCTIONS)
				res = menu.goToMenu(true);

			if (res == Menu::eMenuKeys::CONTINUE)//Continue the game from the same place
			{
				clrscr();
				Board board;
				board.drawBoards();
				player1->drawNewBoard(player1Index);
				player2->drawNewBoard(player2Index);
			}
			else return res;
		}
	}

	//Draw and delete the shape from the screen
	player1->drawShape();
	player2->drawShape();
	Sleep(500);
	player1->drawShape(' ');
	player2->drawShape(' ');

	//Check if one of the players press a key and move the shape with this key
	for (int i = 0; i < GameConfig::numOfKeys; i++)
	{
		int j = (i % GameConfig::numOfMoves);
		if (ch.p1Keys[i] == keyPressed)
			moveShape1 = player1->moveShape((GameConfig::eKeys)j);

		if (ch.p2Keys[i] == keyPressed)
			moveShape2 = player2->moveShape((GameConfig::eKeys)j);
	}

	//Move the shapes down
	moveShape1 = player1->moveShape((GameConfig::eKeys)keyPressed);
	moveShape2 = player2->moveShape((GameConfig::eKeys)keyPressed);

	//If the shape cant move copy to the player board and get a new shape
	if (moveShape1 == false)
		finishPlayerTurn(player1, player1Index, color);

	if (moveShape2 == false)
		finishPlayerTurn(player2, player2Index, color);

	return 0;
}

//Copy the shape to player board and get a new shape
void TetrisGame::finishPlayerTurn(Player* player, int index, bool color)
{
	if (player->isBomb())
	{
		player->updateBoardAfterBomb(index);
	}
	else
	{
		player->copyShapeToBoard();
		player->checkIfThereIsFullLine(index);
	}
	player->getRandShape(color);
}

//Show the winner and players scores
void TetrisGame::finishGame(int score1, int score2, bool losePlayer1, bool losePlayer2)
{
	GameConfig config;
	config.setBlackFont();

	clrscr();
	while (_kbhit())
	{
		_getch();
	}

	if (losePlayer1 && losePlayer2)
	{
		if (score1 == score2)
			cout << "It's a draw!\n" << endl;
		else if (score1 > score2)
			cout << "Player 2 wins!\n" << endl;
		else cout << "Player 1 wins!\n" << endl;
	}
	else if (losePlayer1)
		cout << "Player 2 wins!\n" << endl;
	else
		cout << "Player 1 wins!\n" << endl;

	cout << "Player 1 score: " << score1 << endl;
	cout << "Player 2 score: " << score2 << endl;

	cout << "Press any key for menu";

	//Wait for pressed key and then open the menu
	int keyPressed = noKeyPressed;

	keyPressed = _getch();
	while (keyPressed == noKeyPressed)
		keyPressed = _getch();

	deletePlayers();
	changeGameStatus(false);
}

char TetrisGame::chooseComputerLevel(int playerNum)
{
	cout << "Choose computer player " << playerNum << " level-" << endl;
	cout << "(a) BEST" << endl;
	cout << "(b) GOOD" << endl;
	cout << "(c) NOVICE" << endl;

	char keyPressed = noKeyPressed;
	keyPressed = _getch();
	while (keyPressed == noKeyPressed && keyPressed != ComputerPlayer::eLevel::BEST && keyPressed != ComputerPlayer::eLevel::GOOD && keyPressed != ComputerPlayer::eLevel::NOVICE)
		keyPressed = _getch();

	clrscr();

	if (keyPressed == ComputerPlayer::eLevel::BEST) return ComputerPlayer::eLevel::BEST;
	if (keyPressed == ComputerPlayer::eLevel::GOOD) return ComputerPlayer::eLevel::GOOD;
	if (keyPressed == ComputerPlayer::eLevel::NOVICE) return ComputerPlayer::eLevel::NOVICE;
}