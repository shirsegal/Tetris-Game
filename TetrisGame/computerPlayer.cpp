#include "computerPlayer.h"
#include "gameConfig.h"

void ComputerPlayer::generateMoves()
{
	int moveScore, randMoveNum, shapeNum = shape.getShapeNum(), counter = 0;
	bestMoveScore = -2000;
	bool canMove, rotate = false, counterClock = false, res, down = true;
	int moveArr[arrSIZE] = { 0 };
	bool chooseThisMove = false, makeRandMove = false;
	//moveArr[0] => 0 = left, 1 = right; 
	//moveArr[1] => move before rotate; 
	//moveArr[2] => move after rotate; 
	//moveArr[3] => num of rotations; 
	//moveArr[4] => clockWise = 0, counterClockwise = 1;

	//Check if take a random move 
	if ((level == GOOD && (rand() % 20) == 0) || (level == NOVICE && (rand() % 10) == 0))
	{
		if (shapeNum == (int)Shape::eShapes::squereShape || shapeNum == (int)Shape::eShapes::Bomb)
			randMoveNum = rand() % (GameConfig::GAME_WIDTH - 1); //The num of moves for squere and bomb is ~12
		else randMoveNum = rand() % 47; //The num of option to move for every shape is ~48
		makeRandMove = true;
	}

	Shape temp = shape;
	for (int rotation = 0; rotation < 4; ++rotation) { // Try all 4 rotations

		for (int side = 0; side < 2; ++side)
		{
			moveArr[0] = side;

			if (rotate)
				checkAndRotate(counterClock, rotation, temp, moveArr);

			if (moveArr[3] < rotation)
				keepRotating(rotation, moveArr, temp);

			if (moveArr[3] == rotation)
				rotate = false;

			down = true;
			//check all the possible positions the left side of the board
			while ((side == LEFT && canMoveLeftAndDown(temp, down)) || (side == RIGHT && canMoveRightAndDown(temp, down)))
			{
				counter++;
				if (makeRandMove && (counter == randMoveNum))
					chooseThisMove = true;

				down = moveAndEvaluateShapeSimulator(temp, side, rotate, moveArr, chooseThisMove);
				if (rotate)
				{
					res = checkAndRotate(counterClock, rotation, temp, moveArr);
					if (res && moveArr[3] < rotation)
						keepRotating(rotation, moveArr, temp);
					if (moveArr[3] == rotation)
						rotate = false;
				}
				if (chooseThisMove)
				{
					side = 2;
					break; //choose random the last move, stop the while
				}
			}

			//Bring the shape to the start possition and check the right side
			bringShapeToStartPosition(temp, shape);
			if (rotation != 0)rotate = true;
			resetArr(moveArr);
		}
		rotate = true;
		if (shapeNum == (int)Shape::eShapes::squereShape || shapeNum == (int)Shape::eShapes::Bomb || chooseThisMove)rotation = 3;//The squere and bomb cant rotate so dont continue the for
	}
}

//Copy the start position of shape to temp
void ComputerPlayer::bringShapeToStartPosition(Shape& temp, Shape& shape)
{
	if (!shape.itsBomb())
	{
		for (int i = 0; i < Shape::SIZE; i++)
			temp.getBodyPoint(i).setPoint(shape.getBodyPoint(i).getX(), shape.getBodyPoint(i).getY());
	}
	else temp.getBomb().setPoint(shape.getBomb().getX(), shape.getBomb().getY());
}

//Check if the shapr can move left and down
bool ComputerPlayer::canMoveLeftAndDown(Shape& shape, bool down) const
{
	if (shape.itsBomb())
		return shape.getBomb().getX() > 1 && down;
	else return shape.getBodyPoint(0).getX() > 1 && shape.getBodyPoint(1).getX() > 1 && shape.getBodyPoint(2).getX() > 1 && shape.getBodyPoint(3).getX() > 1 && shape.canMoveLeft() && down;
}

//Check if the shapr can move right and down
bool ComputerPlayer::canMoveRightAndDown(Shape& shape, bool down) const
{
	if (shape.itsBomb())
		return shape.getBomb().getX() < GameConfig::GAME_WIDTH && down;
	else return shape.getBodyPoint(0).getX() < GameConfig::GAME_WIDTH && shape.getBodyPoint(1).getX() < GameConfig::GAME_WIDTH && shape.getBodyPoint(2).getX() < GameConfig::GAME_WIDTH && shape.getBodyPoint(3).getX() < GameConfig::GAME_WIDTH && shape.canMoveRight() && down;
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
	int shapeScore = 0;
	moveShapeDown(shape);

	if (shape.itsBomb())
	{
		Point bomb = shape.getBomb();
		int x = bomb.getX() - 1;
		int y = bomb.getY() - 2;
		shapeScore = evaluateBombScore(x, y);
	}
	else
	{
		// Simulate the placement of the shape and evaluate the board state
		Board simulatedBoard = myBoard;  // Create a copy of the current board state
		copyShapeToBoard(simulatedBoard, shape);


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

	}
	return shapeScore;
}

int ComputerPlayer::evaluateBombScore(int x, int y)
{
	int bombScore = 0;

	for (int j = getMin(GameConfig::GAME_HEIGHT - 1, y + 4); j >= checkMax(0, y - 4); j--)
	{
		for (int i = checkMax(0, x - 4); i < getMin(GameConfig::GAME_WIDTH, x + 5); i++)
		{
			if (myBoard.getBoardYX(j, i) != ' ')
			{
				bombScore++;
			}
		}
	}
	return bombScore;
}

int ComputerPlayer::getMin(int a, int b) const
{
	if (a < b)
	{
		return a;
	}
	return b;
}

int ComputerPlayer::checkMax(int a, int b) const
{
	if (a > b)
		return a;
	else
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
	if (!counterClock && shape.rotateClockwise(getIndex()))
	{
		moveArr[3] += 1;
		return true;
	}

	if ((counterClock || rotation == 0) && shape.rotateCounterClockwise(getIndex()))
	{
		moveArr[4] = COUNTERCLOCKWISE;
		moveArr[3] += 1;
		counterClock = true;
		return true;
	}
	return false;
}

//Move the simulator shape left/right
bool ComputerPlayer::moveAndEvaluateShapeSimulator(Shape& temp, int side, bool rotate, int moveArr[arrSIZE], bool chooseRand)
{
	int moveScore, x, newX, newY;
	bool canMove;

	if (side == LEFT)
	{
		canMove = temp.canMoveLeft() && !temp.reachExistingShape() && !temp.inBottom();//Check if the shape can move down and left
		x = -1;
	}
	if (side == RIGHT)
	{
		canMove = temp.canMoveRight() && !temp.reachExistingShape() && !temp.inBottom();//Check if the shape can move down and right
		x = 1;
	}
	if (canMove)
	{
		if (temp.itsBomb())
		{
			Point& pBomb = temp.getBomb();
			newX = pBomb.getX() + x;
			newY = pBomb.getY() + 1;
			pBomb.setXY(newX, newY);
		}
		else
		{
			for (int i = 0; i < Shape::SIZE; i++)//Move shpe left and down 
			{
				newX = temp.getBodyPoint(i).getX() + x;
				newY = temp.getBodyPoint(i).getY() + 1;
				Point& p = temp.getBodyPoint(i);
				p.setXY(newX, newY);
			}
		}
		updateMoveArr(moveArr);
		if (!rotate)
		{
			moveScore = evaluateMove(temp);
			if (moveScore > bestMoveScore || chooseRand)
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
		while (moveArr[3] < i && shape.rotateClockwise(getIndex()))
		{
			moveShapeOneDown(shape);
			moveArr[3] += 1;
		}
	else
		while (moveArr[3] < i && shape.rotateCounterClockwise(getIndex()))
		{
			moveShapeOneDown(shape);
			moveArr[3] += 1;
		}
}

//Move theshape one row down
void ComputerPlayer::moveShapeOneDown(Shape& shape)
{
	if (!shape.inBottom() && !shape.reachExistingShape())
	{
		if (shape.itsBomb())
		{			
			shape.setBomb(shape.getBomb().getX(), shape.getBomb().getY() + 1);
		}
		else
		{
			for (int i = 0; i < Shape::SIZE; ++i)
				shape.getBodyPoint(i).setPoint(shape.getBodyPoint(i).getX(), shape.getBodyPoint(i).getY() + 1);
		}
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

		int ch = _shape.getColor();
		_board.setBoardYX(y, x, ch);
	}
}


//Move the shape according to the best move arr and move the shape down
bool ComputerPlayer::moveShape(GameConfig::eKeys key)
{
	int k = noKeyPressed;
	bool res = true;

	if (bestMove[1] != 0)
	{
		if (bestMove[0] == 0)
			k = (int)GameConfig::eKeys::LEFT;
		else
			k = (int)GameConfig::eKeys::RIGHT;
		bestMove[1] -= 1;
	}
	else if (bestMove[3] != 0)
	{
		if (bestMove[4] == 0)
			k = (int)GameConfig::eKeys::ROTATE_CLOCKWISE;
		else
			k = (int)GameConfig::eKeys::ROTATE_COUNTER_CLOCKWISE;
		bestMove[3] -= 1;
	}
	else if (bestMove[2] != 0)
	{
		if (bestMove[0] == 0)
			k = (int)GameConfig::eKeys::LEFT;
		else
			k = (int)GameConfig::eKeys::RIGHT;
		bestMove[2] -= 1;
	}

	if (k != noKeyPressed)
		res = shape.move((GameConfig::eKeys)k, getIndex());//Move shape with the key from the best Movearr

	if (!res)return res;

	return shape.move((GameConfig::eKeys)noKeyPressed, getIndex());//Move shape down
}