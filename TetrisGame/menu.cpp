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
	case (int)eMenuKeys::START_HvsH:
		clrscr();
		return game.startNewGame(checkIfPlayWithColor(), 'h', 'h');//Start a game human VS human
		break;
	case (int)eMenuKeys::START_HvsC:
		clrscr();
		return game.startNewGame(checkIfPlayWithColor(), 'h', 'c');//Start a game human VS computer
		break;
	case (int)eMenuKeys::START_CvsC:
		clrscr();
		return game.startNewGame(checkIfPlayWithColor(), 'c', 'c');//Start a game computer VS computer
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
	cout << "(1) Start a new game - Human vs Human" << endl;
	cout << "(2) Start a new game - Human vs Computer" << endl;
	cout << "(3) Start a new game - Computer vs Computer" << endl;
	if (gamePaused == true)
		cout << "(4) Continue a paused game" << endl;
	cout << "(8) Present instructions and keys" << endl;
	cout << "(9) EXIT" << endl;
};

//Check if the player choose a valid menu key
bool Menu::isValidKey(char playerChoice, bool gamePaused)
{
	return playerChoice == START_HvsH || playerChoice == START_HvsC || playerChoice == START_CvsC || (playerChoice == CONTINUE && gamePaused) || playerChoice == INSTRUCTIONS || playerChoice == EXIT;
}


bool Menu::checkIfPlayWithColor()
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
