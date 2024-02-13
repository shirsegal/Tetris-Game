#pragma once
class Menu
{

public:
	enum eMenuKeys { START_NO_COLORS = '1', CONTINUE = '2', START_COLORS = '3', INSTRUCTIONS = '8', EXIT = '9' };
	int goToMenu(bool gamePaused);
	void printMenu(bool gamePaused);
	bool isValidKey(char playerChoice, bool gamePaused);
};

