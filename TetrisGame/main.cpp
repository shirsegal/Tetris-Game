#include <conio.h> // for kbhit+getch
#include <iostream>
#include <Windows.h> // for Sleep and colors
#include "Board.h"
#include "tetrisGame.h"

using namespace std;

void main()
{
	srand(time(NULL));
	TetrisGame game;

	if (!game.runGame()) {
		return; // Exit the program
	}

	return;
}