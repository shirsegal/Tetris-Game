#include "computerPlayer.h"
#include "gameConfig.h"

void ComputerPlayer::generateMoves()
{
	Shape temp = shape;
	bestMoveScore = 0;
	int moveScore;
	bool canMove, rotate = false, counterClock = false, res;
	int moveArr[arrSIZE] = { 0 };
	//moveArr[0] => 0 = left, 1 = right; 
	//moveArr[1] => move before rotate; 
	//moveArr[2] => move after rotate; 
	//moveArr[3] => num of rotations; 
	//moveArr[4] => clockWise = 0, counterClockwise = 1; 

//Check all the possible moves going left
	for (int rotation = 0; rotation < 4; ++rotation) { // Try all 4 rotations

		checkAndRotate(counterClock, rotation, shape, moveArr);
		if (moveArr[3] < rotation)
			keepRotating(rotation, moveArr, temp);

		//check all the possible positionin the left side of the board
		while (temp.getBodyPoint(0).getX() > 0 && temp.getBodyPoint(1).getX() > 0 && temp.getBodyPoint(2).getX() > 0 && temp.getBodyPoint(3).getX() > 0)
		{
			moveAndEvaluateShapeSimulator(temp, LEFT, rotate, moveArr);
			if (rotate)
				res = checkAndRotate(counterClock, rotation, shape, moveArr);
			if (res && moveArr[3] < rotation)
				keepRotating(rotation, moveArr, temp);
		}

		//Bring the shape to the start possition and check the right side
		for (int i = 0; i < Shape::SIZE; i++)
			temp.getBodyPoint(i).setPoint(shape.getBodyPoint(i).getX(), shape.getBodyPoint(i).getY());

		//Check all the possible moves going right
		while (temp.getBodyPoint(0).getX() < GameConfig::GAME_WIDTH && temp.getBodyPoint(1).getX() < GameConfig::GAME_WIDTH && temp.getBodyPoint(2).getX() < GameConfig::GAME_WIDTH && temp.getBodyPoint(3).getX() < GameConfig::GAME_WIDTH)
		{
			moveAndEvaluateShapeSimulator(temp, RIGHT, rotate, moveArr);
			res = checkAndRotate(counterClock, rotation, shape, moveArr);
			if (res && moveArr[3] < rotation)
				keepRotating(rotation, moveArr, temp);
		}
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
	Board simulatedBoard = myBoard;  // Create a copy of the current board state

	int shapeScore = 0;
	moveShapeDown(shape);

	int numOfRows = getTheLowestY(shape);
	int theHighestY = getTheHighestY(shape);

	// 1. Height of the Placement
	shapeScore -= (GameConfig::GAME_HEIGHT - numOfRows);

	// 2. Number of Lines Cleared
	int linesCleared = simulatedBoard.checkIfThereIsFullLine(numOfRows); //בודקת שורות ריקות
	shapeScore += linesCleared * 10;

	// 3. Number of Holes Created
	int holesCreated = simulatedBoard.countHoles(theHighestY); //בודקת חורים
	shapeScore -= holesCreated * 5;

	// 4. Smoothness of the Surface
	int surfaceRoughness = simulatedBoard.calculateBoardSurface(); //בודקת כמה חלק השטח - הפרשי גבהים
	shapeScore -= surfaceRoughness * 3;

	// 5. Proximity to Edges
	int x = shape.getLeftmostEdge();
	int distanceToEdges = getMin(x, (GameConfig::GAME_WIDTH - x - 1)); //עדיף שלא יהיה קרוב לקצוות
	shapeScore -= distanceToEdges * 2;

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

void ComputerPlayer::copyAndResetArr(int from[arrSIZE], int moveScore)
{
	bestMoveScore = moveScore;
	for (int i = 0; i < arrSIZE; i++)
		bestMove[i] = from[i];

	for (int i = 0; i < arrSIZE; i++)
		from[i] = 0;
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
bool ComputerPlayer::checkAndRotate(bool& counterClock, int rotation, Shape shape, int moveArr[arrSIZE])
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
void ComputerPlayer::moveAndEvaluateShapeSimulator(Shape temp, int side, bool rotate, int moveArr[arrSIZE])
{
	int moveScore;
	bool canMove = (temp.canMoveLeft() && !temp.reachExistingShape() && !temp.inBottom());//Check if the shape can move down and left
	if (canMove)
	{
		for (int i = 0; i < Shape::SIZE; i++)//Move shpe left and down 
		{
			temp.getBodyPoint(i).setPoint(temp.getBodyPoint(i).getX() - 1, temp.getBodyPoint(i).getY() + 1);
			updateMoveArr(moveArr);
		}
		if (!rotate)//!rotate && rotation != 0 היה ככה לפני לראות אחר כך אם באמת צריך
		{
			moveScore = evaluateMove(temp);
			if (moveScore > bestMoveScore)
				copyAndResetArr(moveArr, moveScore);
		}
	}
}

//Rotate the shape untill it cant rotate or rotate as num of index
void ComputerPlayer::keepRotating(int& i, int moveArr[arrSIZE], Shape shape)
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
void ComputerPlayer::moveShapeOneDown(Shape shape)
{
	if (!shape.inBottom() && !shape.reachExistingShape())
		for (int i = 0; i < Shape::SIZE; ++i)
			shape.getBodyPoint(i).setPoint(shape.getBodyPoint(i).getX(), shape.getBodyPoint(i).getY() + 1);
}