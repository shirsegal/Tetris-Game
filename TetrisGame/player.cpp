#include "player.h"
#include "gameConfig.h"
#include "Windows.h"

//The function init the player board
void Player::initPlayerBoard()
{
	myBoard.initPlayerBoard(); //הייתה פה פונקציה שהעברתי ללוח
}

//Copy the shape to the playres board
void Player::copyShapeToBoard()
{
	Point currBody;
	for (int i = 0; i < Shape::SIZE; i++)
	{
		currBody = shape.getBodyPoint(i);

		int x = currBody.getX() - 1;
		int y = currBody.getY() - 2;

		int ch = shape.color;
		myBoard.setBoardYX(y, x, ch);
	}
}

//Check if player lose the game
bool Player::loseGame()
{
	return myBoard.loseGame();
}

//The function get the lowest Y of the shape
int Player::getTheLowestY()
{
	Point currBody = shape.getBodyPoint(0);
	int theLowest = currBody.getY();
	for (int i = 1; i < Shape::SIZE; i++)
	{
		currBody = shape.getBodyPoint(i);
		if (theLowest < currBody.getY())
			theLowest = currBody.getY();
	}
	return theLowest;
}

void Player::checkIfThereIsFullLine(int playerIndex)
{
	int returnScore = 0;
	int numOfRows = getTheLowestY();
	returnScore = myBoard.checkIfThereIsFullLine(numOfRows);
	if (returnScore > 0)
		drawNewBoard(playerIndex);
	score += returnScore;
}


//The function draw the board on the screen
void Player::drawNewBoard(int playerIndex) //אפשר להעביר אותה ואת דרואו אולי גם לבורד
{
	for (int i = 0; i < GameConfig::GAME_HEIGHT; i++)
	{
		for (int j = 0; j < GameConfig::GAME_WIDTH; j++)
		{
			//gameBoard[i][j] בורד קודם	
			draw(myBoard.getBoardYX(i, j), j + 1, i + 2, playerIndex);
		}
	}
}

//The function draw all the shapes on the board in their location
void Player::draw(int ch, int _x, int _y, int playerIndex)
{
	bool shape = true;

	if (ch == (int)' ')//Check if the point in bord is free -> draw black point
	{
		ch = GameConfig::BLACK_COLOR;
		shape = false;
	}

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), ch);
	int x_ = _x + (GameConfig::GAME_WIDTH + GameConfig::PLAYERS_SEPERATION + 2) * playerIndex;
	gotoxy(x_, _y);

	if (shape)
		cout << '#';
	else cout << ' ';
}








