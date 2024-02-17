#pragma once
class Menu
{

	bool checkIfPlayWithColor();
	void printInstructions();

public:
	enum eMenuKeys { START_HvsH = '1', START_HvsC = '2', START_CvsC = '3', CONTINUE = '4', INSTRUCTIONS = '8', EXIT = '9' };
	int goToMenu(bool gamePaused);
	void printMenu(bool gamePaused);
	bool isValidKey(char playerChoice, bool gamePaused);
};

