#include "tetrisGame.h"
#include "Board.h"
#include "menu.h"
#include "player.h"
#include "shape.h"
#include "gameConfig.h"
#include <conio.h> // for kbhit+getch
#include <iostream>
#include <Windows.h> 

using namespace std;

//Open the Tetris game
bool TetrisGame::runGame()
{
	int res;
	Menu menu;
	res = menu.goToMenu(false);

	// Check if the game should continue based on the menu interaction
	if (res == (int)Menu::eMenuKeys::EXIT)
		return false;

	return true;
}


//Start a new Tetris game with 2 players
int TetrisGame::startNewGame(bool color)
{
	Board board;
	board.drawBoards();

	Player player1(0);
	Player player2(1);

	bool losePlayer1 = false;
	bool losePlayer2 = false;

	player1.getRandShape(color);
	player2.getRandShape(color);

	//Continue the game untill we have a winner or someone stop the game
	while (!losePlayer1 && !losePlayer2)
	{
		int res = moveShape(player1, player2, color);
		if (res == Menu::eMenuKeys::EXIT)
			return res;

		losePlayer1 = player1.loseGame();
		losePlayer2 = player2.loseGame();
	}


	// Determine the winner
	if (losePlayer1 || losePlayer2)
		finishGame(player1.getScore(), player2.getScore(), losePlayer1, losePlayer2);

}

//Give a shape for every player and move it down and with the player keys untill it touch the bottom
int TetrisGame::moveShape(Player& player1, Player& player2, bool color)
{
	GameConfig ch;
	GameConfig key;

	int player1Index = player1.getIndex();
	int player2Index = player2.getIndex();
	int keyPressed = noKeyPressed;
	bool canRight2 = true, canLeft2 = true, canRight1 = true, canLeft1 = true, moveShape2 = true, moveShape1 = true;

	if (_kbhit())
	{
		keyPressed = _getch();
		if (keyPressed == 27)//Open the main menu
		{
			Menu menu;
			int res = menu.goToMenu(true);
			if (res == Menu::eMenuKeys::INSTRUCTIONS)
				res = menu.goToMenu(true);

			if (res == Menu::eMenuKeys::CONTINUE)//Continue the game from the same place
			{
				clrscr();
				Board board;
				board.drawBoards();
				player1.drawNewBoard(player1Index);
				player2.drawNewBoard(player2Index);
			}
			else if (res == Menu::eMenuKeys::EXIT)//Exit game
				return res;
		}
	}

	//Draw and delete the shpe from the screen
	player1.drawShape();
	player2.drawShape();
	Sleep(500);
	player1.drawShape(' ');
	player2.drawShape(' ');


	//Check if one of the players press a key and move the shape with this key
	for (int i = 0; i < GameConfig::numOfKeys; i++)
	{

		int j = (i % GameConfig::numOfMoves);
		if (ch.p1Keys[i] == keyPressed)
			moveShape1 = player1.moveShape((GameConfig::eKeys)j);

		if (ch.p2Keys[i] == keyPressed)
			moveShape2 = player2.moveShape((GameConfig::eKeys)j);
	}


	//Move the shapes down
	moveShape1 = player1.moveShape((GameConfig::eKeys)keyPressed);
	moveShape2 = player2.moveShape((GameConfig::eKeys)keyPressed);

	//If the shape cant move copy to the player board and get a new shape
	if (moveShape1 == false)
	{
		player1.copyShapeToBoard();
		player1.checkIfThereIsFullLine(player1Index);
		player1.getRandShape(color);
	}

	if (moveShape2 == false)
	{
		player2.copyShapeToBoard();
		player2.checkIfThereIsFullLine(player2Index);
		player2.getRandShape(color);
	}
	return 0;
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

	Menu menu;
	menu.goToMenu(false);
}