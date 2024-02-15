#include "computerPlayer.h"
#include "gameConfig.h"

void ComputerPlayer::generateMoves()
{
	Shape temp = shape;
	bestMoveScore = -2000;
	int moveScore;
	bool canMove, rotate = false, counterClock = false, res, down = true;
	int moveArr[arrSIZE] = { 0 };
	//moveArr[0] => 0 = left, 1 = right; 
	//moveArr[1] => move before rotate; 
	//moveArr[2] => move after rotate; 
	//moveArr[3] => num of rotations; 
	//moveArr[4] => clockWise = 0, counterClockwise = 1; 

//Check all the possible moves going left
	for (int rotation = 0; rotation < 4; ++rotation) { // Try all 4 rotations

		if (rotate)
			checkAndRotate(counterClock, rotation, temp, moveArr);

		if (moveArr[3] < rotation)
			keepRotating(rotation, moveArr, temp);

		if (moveArr[3] == rotation)
			rotate = false;

		down = true;
		//check all the possible positions the left side of the board
		while (temp.getBodyPoint(0).getX() > 1 && temp.getBodyPoint(1).getX() > 1 && temp.getBodyPoint(2).getX() > 1 && temp.getBodyPoint(3).getX() > 1 && temp.canMoveLeft() && down)
		{
			down = moveAndEvaluateShapeSimulator(temp, LEFT, rotate, moveArr);
			if (rotate)
			{
				res = checkAndRotate(counterClock, rotation, temp, moveArr);
				if (res && moveArr[3] < rotation)
					keepRotating(rotation, moveArr, temp);
				if (moveArr[3] == rotation)
					rotate = false;
			}
		}

		//Bring the shape to the start possition and check the right side
		for (int i = 0; i < Shape::SIZE; i++)
			temp.getBodyPoint(i).setPoint(shape.getBodyPoint(i).getX(), shape.getBodyPoint(i).getY());
		if (rotation != 0)rotate = true;
		resetArr(moveArr);

		down = true;

		if (rotate)
			checkAndRotate(counterClock, rotation, temp, moveArr);

		if (moveArr[3] < rotation)
			keepRotating(rotation, moveArr, temp);

		if (moveArr[3] == rotation)
			rotate = false;

		//Check all the possible moves going right
		moveArr[0] = 1;
		while (temp.getBodyPoint(0).getX() < GameConfig::GAME_WIDTH && temp.getBodyPoint(1).getX() < GameConfig::GAME_WIDTH && temp.getBodyPoint(2).getX() < GameConfig::GAME_WIDTH && temp.getBodyPoint(3).getX() < GameConfig::GAME_WIDTH && temp.canMoveRight() && down)
		{
			down = moveAndEvaluateShapeSimulator(temp, RIGHT, rotate, moveArr);
			if (rotate)
			{
				res = checkAndRotate(counterClock, rotation, shape, moveArr);
				if (res && moveArr[3] < rotation)
					keepRotating(rotation, moveArr, temp);
				if (moveArr[3] == rotation)
					rotate = false;
			}
		}
		//Bring the shape to the start possition and check the right side
		for (int i = 0; i < Shape::SIZE; i++)
			temp.getBodyPoint(i).setPoint(shape.getBodyPoint(i).getX(), shape.getBodyPoint(i).getY());
		resetArr(moveArr);
		rotate = true;
	}
}

//The function get the lowest Y of the shape
int ComputerPlayer::getTheLowestY(Shape& shape)
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

//The function get the lowest Y of the shape
int ComputerPlayer::getTheHighestY(Shape& shape)
{
	Point currBody;
	int theHighest = (shape.getBodyPoint(0)).getY();
	for (int i = 1; i < Shape::SIZE; i++)
	{
		currBody = shape.getBodyPoint(i);
		if (theHighest > currBody.getY())
			theHighest = currBody.getY();
	}
	return theHighest;
}

int ComputerPlayer::evaluateMove(Shape shape)
{
	// Simulate the placement of the shape and evaluate the board state
	moveShapeDown(shape);
	Board simulatedBoard = myBoard;  // Create a copy of the current board state
	copyShapeToBoard(simulatedBoard, shape);
	int shapeScore = 0;

	int numOfRows = getTheLowestY(shape);
	int theHighestY = getTheHighestY(shape);

	// 1. Height of the Placement
	shapeScore -= (GameConfig::GAME_HEIGHT - numOfRows);

	// 2. Number of Lines Cleared
	int linesCleared = simulatedBoard.checkIfThereIsFullLine(numOfRows); //בודקת שורות ריקות
	shapeScore += linesCleared * 100000;

	// 3. Number of Holes Created
	int holesCreated = 0;
	int _x, y;
	for (size_t i = 0; i < Shape::SIZE; i++)
	{
		_x = (shape.getBodyPoint(i)).getX() - 1;
		y = (shape.getBodyPoint(i)).getY() - 2;
		holesCreated += simulatedBoard.countHoles(y, _x); //בודקת חורים
	}
	shapeScore -= holesCreated * 50;

	// 4. Smoothness of the Surface
	int surfaceRoughness = simulatedBoard.calculateBoardSurface(); //בודקת כמה חלק השטח - הפרשי גבהים
	shapeScore -= surfaceRoughness * 3;

	// 5. Proximity to Edges
	int x = shape.getLeftmostEdge();
	int distanceToEdges = getMin(x, (GameConfig::GAME_WIDTH - x - 1)); //עדיף שלא יהיה קרוב לקצוות
	shapeScore += distanceToEdges * 2;

	return shapeScore;
}

int ComputerPlayer::getMin(int& a, int b) const
{
	if (a < b)
	{
		return a;
	}
	return b;
}

void ComputerPlayer::copyArr(int from[arrSIZE], int moveScore)
{
	bestMoveScore = moveScore;
	for (int i = 0; i < arrSIZE; i++)
		bestMove[i] = from[i];
}

//Update the move arr, check if movebefore or after rotation
void ComputerPlayer::updateMoveArr(int arr[arrSIZE])
{
	if (arr[3] == 0)//check if move right before or after rotation
		arr[1] += 1;
	else arr[2] += 1;
}

//Move the shape all the way down
void ComputerPlayer::moveShapeDown(Shape& shape)
{
	while (!shape.inBottom() && !shape.reachExistingShape())
		moveShapeOneDown(shape);
}

//Resetthe move arr to 0
void ComputerPlayer::resetArr(int arr[arrSIZE])
{
	for (int i = 0; i < arrSIZE; i++)
		arr[i] = 0;
}

//check if the shape can rotate if can rotate the shape
bool ComputerPlayer::checkAndRotate(bool& counterClock, int rotation, Shape& shape, int moveArr[arrSIZE])
{
	if (!counterClock && shape.rotateClockwise(index))
	{
		moveArr[3] += 1;
		return true;
	}

	if ((counterClock || rotation == 0) && shape.rotateCounterClockwise(index))
	{
		moveArr[4] = COUNTERCLOCKWISE;
		moveArr[3] += 1;
		counterClock = true;
		return true;
	}
	return false;
}

//Move the simulator shape left/right
bool ComputerPlayer::moveAndEvaluateShapeSimulator(Shape& temp, int side, bool rotate, int moveArr[arrSIZE])
{
	int moveScore;
	bool canMove;
	if (side == LEFT) canMove = temp.canMoveLeft() && !temp.reachExistingShape() && !temp.inBottom();//Check if the shape can move down and left
	if (side == RIGHT) canMove = temp.canMoveRight() && !temp.reachExistingShape() && !temp.inBottom();//Check if the shape can move down and right

	if (canMove)
	{
		for (int i = 0; i < Shape::SIZE; i++)//Move shpe left and down 
		{
			int newX = temp.getBodyPoint(i).getX() + side;
			int newY = temp.getBodyPoint(i).getY() + 1;
			Point& p = temp.getBodyPoint(i);
			p.setXY(newX, newY);
		}
		updateMoveArr(moveArr);
		if (!rotate)//!rotate && rotation != 0 היה ככה לפני לראות אחר כך אם באמת צריך
		{
			moveScore = evaluateMove(temp);
			if (moveScore > bestMoveScore)
				copyArr(moveArr, moveScore);
		}
		return true;
	}
	return false;
}

//Rotate the shape untill it cant rotate or rotate as num of index
void ComputerPlayer::keepRotating(int& i, int moveArr[arrSIZE], Shape& shape)
{
	if (moveArr[4] == CLOCKWISE)
		while (moveArr[3] < i && shape.rotateClockwise(index))
		{
			moveShapeOneDown(shape);
			moveArr[3] += 1;
		}
	else
		while (moveArr[3] < i && shape.rotateCounterClockwise(index))
		{
			moveShapeOneDown(shape);
			moveArr[3] += 1;
		}
}

//Move theshape one row down
void ComputerPlayer::moveShapeOneDown(Shape& shape)
{
	if (!shape.inBottom() && !shape.reachExistingShape())
		for (int i = 0; i < Shape::SIZE; ++i)
			shape.getBodyPoint(i).setPoint(shape.getBodyPoint(i).getX(), shape.getBodyPoint(i).getY() + 1);
}


//Check if player lose the game
bool ComputerPlayer::loseGame()
{
	return myBoard.loseGame();
}

//The function init the player board
void ComputerPlayer::initPlayerBoard()
{
	myBoard.initPlayerBoard(); //הייתה פה פונקציה שהעברתי ללוח
}

//Copy the shape to the playres board
void ComputerPlayer::copyShapeToBoard()
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

//Copy the shape to the playres board
void ComputerPlayer::copyShapeToBoard(Board& _board, Shape _shape)
{
	Point currBody;
	for (int i = 0; i < Shape::SIZE; i++)
	{
		currBody = _shape.getBodyPoint(i);

		int x = currBody.getX() - 1;
		int y = currBody.getY() - 2;

		int ch = _shape.color;
		_board.setBoardYX(y, x, ch);
	}
}


//The function draw the board on the screen
void ComputerPlayer::drawNewBoard(int playerIndex) //אפשר להעביר אותה ואת דרואו אולי גם לבורד
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
void ComputerPlayer::draw(int ch, int _x, int _y, int playerIndex)
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

void ComputerPlayer::checkIfThereIsFullLine(int playerIndex)
{
	int returnScore = 0;
	int numOfRows = getTheLowestY();
	returnScore = myBoard.checkIfThereIsFullLine(numOfRows);
	if (returnScore > 0)
		drawNewBoard(playerIndex);
	score += returnScore;
}

//The function get the lowest Y of the shape
int ComputerPlayer::getTheLowestY()
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

bool ComputerPlayer::moveShape2()
{
	bool res = true;
	while (bestMove[1] != 0)
	{
		if (bestMove[0] == 0)
			res = shape.move(GameConfig::eKeys::LEFT, index);
		else
			res = shape.move(GameConfig::eKeys::RIGHT, index);
		bestMove[1] -= 1;
	}
	while (bestMove[3] != 0)
	{
		if (bestMove[4] == 0)
			res = shape.move(GameConfig::eKeys::ROTATE_CLOCKWISE, index);
		else
			res = shape.move(GameConfig::eKeys::ROTATE_COUNTER_CLOCKWISE, index);
		bestMove[3] -= 1;
	}
	while (bestMove[2] != 0)
	{
		if (bestMove[0] == 0)
			res = shape.move(GameConfig::eKeys::LEFT, index);
		else
			res = shape.move(GameConfig::eKeys::RIGHT, index);
		bestMove[2] -= 1;
	}
	return res;
}
//moveArr[0] => 0 = left, 1 = right; 
//moveArr[1] => move before rotate; 
//moveArr[2] => move after rotate; 
//moveArr[3] => num of rotations; 
//moveArr[4] => clockWise = 0, counterClockwise = 1; 