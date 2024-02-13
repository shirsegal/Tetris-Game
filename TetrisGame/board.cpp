#include "Board.h"

//The function draw the board frame
void Board::drawBorder(int x, int y)
{
	for (int col = 0; col <= GameConfig::GAME_WIDTH + 1; col++)
	{
		gotoxy(col + x, y);
		cout << "*";

		gotoxy(col + x, GameConfig::GAME_HEIGHT + 2);
		cout << "*";
	}

	for (int row = 1; row <= GameConfig::GAME_HEIGHT + 1; row++)
	{
		gotoxy(x, row + y);
		cout << "*";

		gotoxy(GameConfig::GAME_WIDTH + x + 1, row + y);
		cout << "*";
	}
}

//The function init the player board
void Board::initPlayerBoard()
{
	for (int i = 0; i < GameConfig::GAME_HEIGHT; i++)
	{
		for (int j = 0; j < GameConfig::GAME_WIDTH; j++)
		{
			gameBoard[i][j] = ' ';
		}
	}
}

//Init a new empty board //TODO void Player::initPlayerBoard()
void Board::initBoard(int board[GameConfig::GAME_HEIGHT][GameConfig::GAME_WIDTH])
{
	for (int i = 0; i < GameConfig::GAME_HEIGHT; i++)
	{
		for (int j = 0; j < GameConfig::GAME_WIDTH; j++)
		{
			board[i][j] = ' ';
		}
	}
}

//The function draw the frame for two players
void Board::drawBoards()
{
	cout << "Player 1" << endl;
	drawBorder(0, 1);

	gotoxy(GameConfig::GAME_WIDTH + 2 + GameConfig::PLAYERS_SEPERATION, 0);
	cout << "Player 2" << endl;
	drawBorder(GameConfig::GAME_WIDTH + 2 + GameConfig::PLAYERS_SEPERATION, 1);

}

//Check if player lose the game
bool Board::loseGame()
{
	for (int i = 0; i < GameConfig::GAME_WIDTH; i++)
	{
		char ch = gameBoard[0][i];
		if (ch != ' ')
			return true;
	}
	return false;
}

//The function check if there is a full line
int Board::checkIfThereIsFullLine(int _numOfRows)
{
	int score = 0;
	int count;
	int countFull = 0;
	int numOfRows;
	int y;
	int tempBoard[GameConfig::GAME_HEIGHT][GameConfig::GAME_WIDTH];
	initBoard(tempBoard);
	numOfRows = _numOfRows;
	y = numOfRows;
	for (int i = numOfRows; (i - 2) >= 0; i--)
	{
		count = 0;
		for (int j = 0; j < GameConfig::GAME_WIDTH; j++)
		{
			if (gameBoard[i - 2][j] == ' ')
			{
				j = GameConfig::GAME_WIDTH;
			}
			else
				count++;
		}
		if (count < GameConfig::GAME_WIDTH)
		{
			if (countFull > 0)
			{
				for (int k = 0; k < GameConfig::GAME_WIDTH; k++)
				{
					tempBoard[numOfRows - 2][k] = gameBoard[i - 2][k];
				}
				numOfRows--;
			}
		}
		else
			countFull++;
	}
	if (countFull > 0)
	{
		score += 100;
		copyNewBoard(tempBoard, y);
	}
	return score;
}

//The function copy the trmp board after there is a full line that we erase
void Board::copyNewBoard(int board[GameConfig::GAME_HEIGHT][GameConfig::GAME_WIDTH], int y)
{
	for (int i = y; (i - 2) >= 0; i--)
	{
		for (int j = 0; j < GameConfig::GAME_WIDTH; j++)
		{
			gameBoard[i - 2][j] = board[i - 2][j];
		}
	}
}

//The function count the number of holles in board
int Board::countHoles(int theHighestY) const
{
	int countHoles = 0;

	for (int i = theHighestY; (i - 2) < GameConfig::GAME_HEIGHT; i++)
	{
		for (int j = 0; j < GameConfig::GAME_WIDTH; j++)
		{
			if (gameBoard[i - 2][j] == ' ' && gameBoard[i - 3][j] != ' ')
			{
				countHoles++;
			}
		}
	}
	return countHoles;
}

int Board::getColumnHeight(int x) const
{
	for (int y = 0; y < GameConfig::GAME_HEIGHT; ++y) {
		if (gameBoard[y][x] != ' ') {
			// Return the height of the first occupied cell in column x
			return GameConfig::GAME_HEIGHT - y;
		}
	}
	// If the column is completely empty, return 0
	return 0;
}

int Board::calculateBoardSurface() const
{
	int roughness = 0;
	for (int x = 0; x < (GameConfig::GAME_WIDTH - 1); x++) 
	{
		int height1 = getColumnHeight(x);
		int height2 = getColumnHeight(x + 1);
		roughness += abs(height1 - height2);
	}
	return roughness;
}
