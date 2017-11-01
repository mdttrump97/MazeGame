#include<iostream>
#include<stack>
#include<list>
#include<random>
#include<time.h>
#include<windows.h>
#include "Maze.h"
#include "Location.h"

using std::cout;
using std::endl;

Maze::Maze()
{
	generateMaze();
	placeStartPosition();
	generatePaths();
	placeDestination();
}

void Maze::generateMaze()
{
	for (int i = 0; i < ROWS; i++)
	{
		for (int j = 0; j < COLUMNS; j++)
		{
			maze[i][j] = WALL;
		}
	}
}

void Maze::placeStartPosition()
{
	srand((unsigned int)time(NULL));

	// rand() % COLUMNS shrinks the random number to be an allowable maze width.
	int randomColumn = (int)rand() % COLUMNS;

	// The if-else statements are used to ensure that randomColumn is an odd number that is not the edge of the maze.
	if (randomColumn % 2 == 0 && randomColumn != COLUMNS - 1 || randomColumn == 0)
	{
		randomColumn++;
	}
	else if (randomColumn == COLUMNS - 1)
	{
		randomColumn--;
	}

	// rand() % ROWS shrinks the random number to be an allowable maze height.
	int randomRow = (int)rand() % ROWS;

	// The if-else statements are used to ensure that randomRow is an odd number that is not the edge of the maze.
	if (randomRow % 2 == 0 && randomRow != ROWS - 1 || randomRow == 0)
	{
		randomRow++;
	}
	else if (randomRow == ROWS - 1)
	{
		randomRow--;
	}

	startLocation = Location(randomRow, randomColumn);
	playerLocation = startLocation;
	maze[startLocation.row][startLocation.column] = START;
}

void Maze::generatePaths()
{
	Location currentLocation = startLocation;
	std::stack <Location> locationStack;
	do
	{
		// Instead of looping through each possible moves, a list of possible moves 
		// is created in a random order and the front of the list is checked to 
		// decide which move to perform.
		std::list <moveDirection> possibleMoves = {};

		listPossibleMoves(possibleMoves, currentLocation);

		if (!possibleMoves.empty())
		{
			locationStack.push(currentLocation);
			peformRandomPossibleMove(possibleMoves, currentLocation);
		}
		else
		{
			if (!locationStack.empty())
			{
				currentLocation = locationStack.top();
				locationStack.pop();
			}
		}
	} while (!locationStack.empty());
}

void Maze::listPossibleMoves(list<moveDirection>& possibleMoves, Location currentLocation)
{
	// Make a list of all possible moves in a random order by either
	// inserting a move into the front or back of the possible moves list

	// A valid move is two steps away and is a wall character
	if (currentLocation.column + 2 < COLUMNS && maze[currentLocation.row][currentLocation.column + 2] == WALL)
	{
		int randomInsertionSelection = rand() % 2;
		if (randomInsertionSelection == 0)
		{
			possibleMoves.push_front(RIGHT);
		}
		else
		{
			possibleMoves.push_back(RIGHT);
		}
	}
	if (currentLocation.column - 2 > 0 && maze[currentLocation.row][currentLocation.column - 2] == WALL)
	{
		int randomInsertionSelection = rand() % 2;
		if (randomInsertionSelection == 0)
		{
			possibleMoves.push_front(LEFT);
		}
		else
		{
			possibleMoves.push_back(LEFT);
		}
	}
	if (currentLocation.row + 2 < ROWS && maze[currentLocation.row + 2][currentLocation.column] == WALL)
	{
		int randomInsertionSelection = rand() % 2;
		if (randomInsertionSelection == 0)
		{
			possibleMoves.push_front(DOWN);
		}
		else
		{
			possibleMoves.push_back(DOWN);
		}
	}
	if (currentLocation.row - 2 > 0 && maze[currentLocation.row - 2][currentLocation.column] == WALL)
	{
		int randomInsertionSelection = rand() % 2;
		if (randomInsertionSelection == 0)
		{
			possibleMoves.push_front(UP);
		}
		else
		{
			possibleMoves.push_back(UP);
		}
	}
}

void Maze::peformRandomPossibleMove(list<moveDirection> possibleMoves, Location& currentLocation)
{
	// Decide which direction to move by getting the first move in the list of possible moves
	switch (possibleMoves.front())
	{
	case RIGHT:
		currentLocation.column++;
		maze[currentLocation.row][currentLocation.column] = PATH;
		currentLocation.column++;
		maze[currentLocation.row][currentLocation.column] = PATH;
		break;

	case LEFT:
		currentLocation.column--;
		maze[currentLocation.row][currentLocation.column] = PATH;
		currentLocation.column--;
		maze[currentLocation.row][currentLocation.column] = PATH;
		break;

	case DOWN:
		currentLocation.row++;
		maze[currentLocation.row][currentLocation.column] = PATH;
		currentLocation.row++;
		maze[currentLocation.row][currentLocation.column] = PATH;
		break;

	case UP:
		currentLocation.row--;
		maze[currentLocation.row][currentLocation.column] = PATH;
		currentLocation.row--;
		maze[currentLocation.row][currentLocation.column] = PATH;
		break;
	}
}

void Maze::placeDestination()
{
	// The destination symbol will be placed at a random location in a path.
	int randomRow = rand() % ROWS;
	int randomColumn = rand() % COLUMNS;

	while (maze[randomRow][randomColumn] != PATH)
	{
		randomRow = rand() % ROWS;
		randomColumn = rand() % COLUMNS;
	}

	destination.row = randomRow;
	destination.column = randomColumn;
	maze[randomRow][randomColumn] = DESTINATION;
}

void Maze::displayMaze()
{
	for (int i = 0; i < ROWS; i++)
	{
		for (int j = 0; j < COLUMNS; j++)
		{
			HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
			if (maze[i][j] == START)
			{
				// 10 sets the start character to be bright green.
				SetConsoleTextAttribute(hConsole, 10);
			}
			else if (maze[i][j] == DESTINATION)
			{
				// 14 sets the destination character to be yellow.
				SetConsoleTextAttribute(hConsole, 14);
			}
			else
			{
				// 7 sets the output to light grey.
				SetConsoleTextAttribute(hConsole, 7);
			}
			cout << maze[i][j];
		}
		cout << endl;
	}
}

void Maze::processKeyInputToMovePlayer()
{
	bool atDestination = false;
	while (!atDestination)
	{
		Location previousLocation = playerLocation;
		switch (getKey())
		{
		case VK_LEFT:
			if (maze[playerLocation.row][playerLocation.column - 1] != WALL)
			{
				playerLocation.column--;
				placePlayer(playerLocation);
				fixPath(previousLocation);
				fixStart(startLocation);
			}
			break;
		case VK_RIGHT:
			if (maze[playerLocation.row][playerLocation.column + 1] != WALL)
			{
				playerLocation.column++;
				placePlayer(playerLocation);
				fixPath(previousLocation);
				fixStart(startLocation);
			}
			break;
		case VK_UP:
			if (maze[playerLocation.row - 1][playerLocation.column] != WALL)
			{
				playerLocation.row--;
				placePlayer(playerLocation);
				fixPath(previousLocation);
				fixStart(startLocation);
			}
			break;
		case VK_DOWN:
			if (maze[playerLocation.row + 1][playerLocation.column] != WALL)
			{
				playerLocation.row++;
				placePlayer(playerLocation);
				fixPath(previousLocation);
				fixStart(startLocation);
			}
			break;
		}
		if (playerLocation.row == destination.row && playerLocation.column == destination.column)
		{
			atDestination = true;
		}

	}
	for (int i = 0; i < 50; i++)
	{
		cout << "\n";
	}
	cout << "*************************" << endl;
	cout << "YAY! YOU SOLVED THE MAZE!" << endl;
	cout << "*************************" << endl;
}

int Maze::getKey()
{
	int result = 0;
	
	short MAX_SHORT = 0x7FFF; //111111111111111
	if (GetAsyncKeyState(VK_LEFT) & MAX_SHORT)
	{
		result = VK_LEFT;
	}
	else if (GetAsyncKeyState(VK_UP) & MAX_SHORT)
	{
		result = VK_UP;
	}
	else if (GetAsyncKeyState(VK_RIGHT) & MAX_SHORT)
	{
		result = VK_RIGHT;
	}
	else if (GetAsyncKeyState(VK_DOWN) & MAX_SHORT)
	{
		result = VK_DOWN;
	}
	return result;
}

void Maze::placePlayer(Location location)
{
	COORD playerCoordinate = {location.column, location.row};
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), playerCoordinate);

	// Hide the console cursor
	CONSOLE_CURSOR_INFO     cursorInfo;
	GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
	cursorInfo.bVisible = false; // Set the cursor visibility
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);

	// 11 sets the Player symbol to bright blue.
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);

	cout << PLAYER;
}

void Maze::fixPath(Location location)
{
	COORD coordinate = { location.column,location.row };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coordinate);
	maze[location.row][location.column] = PATH;
	cout << PATH;
}

void Maze::fixStart(Location location)
{
	COORD coordinate = { location.column, location.row };
	// A value of 10 makes the Start symbol bright green
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coordinate);
	cout << START;
}