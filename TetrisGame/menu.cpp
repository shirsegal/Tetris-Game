#include "menu.h"
#include <iostream>
#include "tetrisGame.h"
#include "goToxy.h"
#include <conio.h>
#include <Windows.h>
using namespace std;

//Open the game menu and run the player choice
int Menu::goToMenu(bool gamePaused)
{
	printMenu(gamePaused);

	TetrisGame game;
	char playerChoice = _getch();
	while (!isValidKey(playerChoice, gamePaused))
		playerChoice = _getch();

	switch (playerChoice)
	{
	case (int)eMenuKeys::START_NO_COLORS://Start a new black and white tetris game
		clrscr();
		return game.startNewGame(false);
		break;

	case (int)eMenuKeys::START_COLORS://Start a new tetris game with colors
		clrscr();
		return game.startNewGame(true);
		break;

	case (int)eMenuKeys::INSTRUCTIONS://Print tetris game instructions and players keys
	{
		clrscr();
		cout << "Tetris Instructions :\n" << endl;

		cout << "Player 1 Controls:" << endl;
		cout << "Left(A) : Move block left." << endl;
		cout << "Right(D) : Move block right." << endl;
		cout << "Rotate Clockwise(S) : Rotate block clockwise." << endl;
		cout << "Rotate Counter Clockwise(W) : Rotate block counter - clockwise." << endl;
		cout << "Drop(X) : Accelerate block descent.\n" << endl;

		cout << "Player 2 Controls :" << endl;
		cout << "Left(J) : Move block left." << endl;
		cout << "Right(L) : Move block right." << endl;
		cout << "Rotate Clockwise(I) : Rotate block clockwise." << endl;
		cout << "Rotate Counter Clockwise(K) : Rotate block counter - clockwise." << endl;
		cout << "Drop(M) : Accelerate block descent.\n" << endl;

		cout << "Objective:" << endl;
		cout << "Clear lines by fitting falling blocks together to create complete horizontal lines.\n" << endl;

		cout << "Enjoy the game!\n" << endl;

		cout << "To exit instructions press ESC" << endl;

		char playerChoice = _getch();

		while (playerChoice != 27)
			playerChoice = _getch();

		return INSTRUCTIONS;
		break;
	}
	case EXIT://Exit the game
	{
		clrscr();
		cout << "Exiting the game..." << endl;
		return EXIT;
		break;
	}
	}

}

//Print the game menu
void Menu::printMenu(bool gamePaused)
{
	clrscr();
	GameConfig config;
	config.setBlackFont();

	cout << "TETRIS GAME\n" << endl;
	cout << "(1) Start a new game - without colors" << endl;
	if (gamePaused == true)
		cout << "(2) Continue a paused game" << endl;
	cout << "(3) Start a new game - with colors" << endl;
	cout << "(8) Present instructions and keys" << endl;
	cout << "(9) EXIT" << endl;
};

//Check if the player choose a valid menu key
bool Menu::isValidKey(char playerChoice, bool gamePaused)
{
	return playerChoice == START_COLORS || (playerChoice == CONTINUE && gamePaused) || playerChoice == START_NO_COLORS || playerChoice == INSTRUCTIONS || playerChoice == EXIT;
}