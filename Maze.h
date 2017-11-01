#pragma once

#include<stack>
#include<list>
#include "Location.h"
using std::list;
using std::stack;
class Maze
{
	public:
		
		Maze();
		void displayMaze();
		void processKeyInputToMovePlayer();

		// ASCII Codes:
		// Wall - Block
		const char WALL = 219; 
		// Path - Space
		const char PATH = 32;
		// Player - P
		const char PLAYER = 80;
		// Destination - X
		const char DESTINATION = 88;
		// Start - S
		const char START = 83;

		// ROWS and COLUMNS must be odd numbers for proper maze path generation
		const int ROWS = 35;
		const int COLUMNS = 75;

	private:
		// maze[ROWS][COLUMNS]
		char maze[35][75] = { 0 };
		Location playerLocation = Location(0,0);
		Location startLocation = Location(0, 0);
		Location destination = Location(0, 0);
		enum moveDirection { UP, DOWN, LEFT, RIGHT };
		void generateMaze();
		void placeStartPosition();
		void generatePaths();
		void listPossibleMoves(list<moveDirection>&, Location);
		void peformRandomPossibleMove(list<moveDirection>, Location&);
		void placeDestination();
		int getKey();
		void placePlayer(Location);
		void fixPath(Location);
		void fixStart(Location);
};
