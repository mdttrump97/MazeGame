#include<iostream>
#include<Windows.h>
#include "Maze.h"

using namespace std;

void main()
{	
	Maze maze;
	maze.displayMaze();
	maze.processKeyInputToMovePlayer();
}