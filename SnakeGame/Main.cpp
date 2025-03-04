#include <iostream>
#include <list>
#include <iterator>
#include <windows.h>
#include <conio.h>

#include "Coordinate.h"

#define SIZEX 20
#define SIZEY 15
#define STARTSIZE 3
using namespace std;

void set_cursor(int, int);
Coordinate findRandomEmptySpace(char** board);

int main()
{
	//	create board
	char* board[SIZEY];
	for (int i = 0; i < SIZEY; i++)
	{
		board[i] = new char[SIZEX];
		for (int j = 0; j < SIZEX; j++)
		{
			board[i][j] = ' ';
		}
	}	

	//	create snake
	list<Coordinate> snake;
	snake.push_front(Coordinate(SIZEY / 2, 3));
	for (int i = 0; i < STARTSIZE; i++)
	{
		board[snake.front().getY()][snake.front().getX()] = '=';
		snake.push_front(Coordinate(snake.front().getY() , snake.front().getX() + 1));
	}
	board[snake.front().getY()][snake.front().getX()] = '>';

	char direction = 'd'; // d = east	a = west	w = north	s = south
	bool isAlive = true;
	list<Coordinate>::iterator iter;
	int score = 0;
	srand(time(0));
	bool isPaused = false;

	//apple generation
	Coordinate apple = findRandomEmptySpace(board);
	board[apple.getY()][apple.getX()] = 'a';

	// portal generation
	Coordinate portalA, portalB;
	bool genDone = false;
	while (!genDone)
	{
		portalA = findRandomEmptySpace(board);
		portalB = findRandomEmptySpace(board);
		if (portalA.getX() != 0 &&			// no portal at wall allowed
			portalA.getX() != SIZEX - 1 &&
			portalA.getY() != 0 &&
			portalA.getY() != SIZEX - 1 &&
			portalB.getX() != 0 &&
			portalB.getX() != SIZEX - 1 &&
			portalB.getY() != 0 &&
			portalB.getY() != SIZEX - 1)
			genDone = true;
	}
	board[portalA.getY()][portalA.getX()] = 'O';
	board[portalB.getY()][portalB.getX()] = 'O';

	//game start
	while (isAlive)
	{
		// 10 steps per second
		Sleep(1000 / 10);

		// check for player input
		while (_kbhit()) 
		{
			char input = _getch();
			bool isDone = false;
			switch (input)
			{
			case 'w':
			case 72:
				if (direction != 's')
					direction = 'w';
				isDone = true;
				break;

			case 's':
			case 80:
				if (direction != 'w')
					direction = 's';
				isDone = true;
				break;

			case 'a':
			case 75:
				if (direction != 'd')
					direction = 'a';
				isDone = true;
				break;

			case 'd':
			case 77:
				if (direction != 'a')
					direction = 'd';
				isDone = true;
				break;
			case 'q':
				isAlive = false;
				isDone = true;
				break;
			case 'p':
				isPaused = true;
				isDone = true;
				break;
			}			
			if (isDone)
				break;
		}

		// game pause
		while (isPaused)
		{
			if (_getch() == 'p')
				isPaused = false;
		}

		//check for wall collision
		switch (direction) 
		{
		case 'd':
			if (snake.front().getX() + 1 == SIZEX)
			{
				isAlive = false;
				break;
			}
			if (snake.front().getY() == portalB.getY() &&
				snake.front().getX() + 1 == portalB.getX())
			{
				snake.push_front(Coordinate(portalA.getY(), portalA.getX() + 1));
				break;
			}
			if (snake.front().getY() == portalA.getY() &&
				snake.front().getX() + 1 == portalA.getX())
			{
				snake.push_front(Coordinate(portalB.getY(), portalB.getX() + 1));
				break;
			}
			snake.push_front(Coordinate(snake.front().getY(), snake.front().getX() + 1));
			break;

		case 'a':
			if (snake.front().getX() - 1 == -1)
			{
				isAlive = false;
				break;
			}
			if (snake.front().getY() == portalB.getY() &&
				snake.front().getX() - 1 == portalB.getX())
			{
				snake.push_front(Coordinate(portalA.getY(), portalA.getX() - 1));
				break;
			}
			if (snake.front().getY() == portalA.getY() &&
				snake.front().getX() - 1 == portalA.getX())
			{
				snake.push_front(Coordinate(portalB.getY(), portalB.getX() - 1));
				break;
			}
			snake.push_front(Coordinate(snake.front().getY(), snake.front().getX() - 1));
			break;

		case 's':
			if (snake.front().getY() + 1 == SIZEY)
			{
				isAlive = false;
				break;
			}
			if (snake.front().getY() + 1== portalB.getY() &&
				snake.front().getX() == portalB.getX())
			{
				snake.push_front(Coordinate(portalA.getY() + 1, portalA.getX()));
				break;
			}
			if (snake.front().getY() + 1 == portalA.getY() &&
				snake.front().getX() == portalA.getX())
			{
				snake.push_front(Coordinate(portalB.getY() + 1, portalB.getX()));
				break;
			}
			snake.push_front(Coordinate(snake.front().getY() + 1, snake.front().getX()));
			break;

		case 'w':
			if (snake.front().getY() - 1 == -1)
			{
				isAlive = false;
				break;
			}
			if (snake.front().getY() - 1 == portalB.getY() &&
				snake.front().getX() == portalB.getX())
			{
				snake.push_front(Coordinate(portalA.getY() - 1, portalA.getX()));
				break;
			}
			if (snake.front().getY() - 1 == portalA.getY() &&
				snake.front().getX() == portalA.getX())
			{
				snake.push_front(Coordinate(portalB.getY() - 1, portalB.getX()));
				break;
			}
			snake.push_front(Coordinate(snake.front().getY() - 1, snake.front().getX()));
			break;
		}
		if (snake.front().getY() > SIZEY - 1 || snake.front().getY() < 0 ||
			snake.front().getX() > SIZEX - 1 || snake.front().getX() < 0)
			throw ("out of bounds");

		//advance snake
		if (isAlive)
		{	
			if (snake.front() != apple)
			{
				board[snake.back().getY()][snake.back().getX()] = ' ';
				snake.pop_back();
			}
			board[(++snake.begin())->getY()][(++snake.begin())->getX()] = '=';
			switch (direction)
			{
			case 'a':
				board[snake.front().getY()][snake.front().getX()] = '<';
				break;
			case 'd':
				board[snake.front().getY()][snake.front().getX()] = '>';
				break;
			case 'w':
				board[snake.front().getY()][snake.front().getX()] = '^';
				break;
			case 's':
				board[snake.front().getY()][snake.front().getX()] = 'v';
				break;
			}			
		}

		//check for collision with self
		for (iter = ++snake.begin(); iter != snake.end(); iter++)
		{
			if (*iter == snake.front())
			{
				isAlive = false;
				break;
			}
		}

		//apple
		if (snake.front() == apple)
		{
			score++;
			apple = findRandomEmptySpace(board);
		}
		board[apple.getY()][apple.getX()] = 'a';

		// print board
		set_cursor(0,0);
		for (int i = 0; i < SIZEX + 2; i++)
			cout << "x ";
		cout << endl;
		for (int i = 0; i < SIZEY; i++)
		{
			cout << "x ";
			for (int j = 0; j < SIZEX; j++)
			{
				cout << board[i][j] << ' ';
			}
			cout << "x " << endl;
		}
		for (int i = 0; i < SIZEX + 2; i++)
			cout << "x ";
		cout << endl << endl;
	}

	cout << endl << "Game Over: " << score << endl;

	Sleep(1000);

	for (int i = 0; i < SIZEY; i++)
	{
		delete board[i];
	}
	// end of program
	return 0;
}

void set_cursor(int x = 0, int y = 0)
{
	HANDLE handle;
	COORD coordinates;
	handle = GetStdHandle(STD_OUTPUT_HANDLE);
	coordinates.X = x;
	coordinates.Y = y;
	SetConsoleCursorPosition(handle, coordinates);
}

Coordinate findRandomEmptySpace(char** board)
{
	Coordinate var;
	bool valid = false;
	while (!valid)
	{
		var.setX(rand() % SIZEX);
		var.setY(rand() % SIZEY);
		if (board[var.getY()][var.getX()] == ' ')
			valid = true;
	}
	return var;
}

//end of file