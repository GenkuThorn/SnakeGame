#include <iostream>
#include <list>
#include <iterator>
#include <windows.h>
#include <conio.h>

#define SIZEX 20
#define STARTSIZE 3
using namespace std;

void set_cursor(int, int);

int main()
{
	//	create board
	char* board[SIZEX];
	for (int i = 0; i < SIZEX; i++)
	{
		board[i] = new char[SIZEX];
		for (int j = 0; j < SIZEX; j++)
		{
			board[i][j] = ' ';
		}
	}

	//	create snake
	//	x is 0 to SIZE-1
	//	y is SIZE * (0 to SIZE-1)
	list<int> snake;
	snake.push_front(3 + SIZEX/2*SIZEX);
	for (int i = 0; i < STARTSIZE; i++)
	{
		board[snake.front() / SIZEX][snake.front() % SIZEX] = 'b';
		snake.push_front(snake.front() + 1);
	}
	board[snake.front() / SIZEX][snake.front() % SIZEX] = 'v';

	char direction = 'd'; // d = east	a = west	w = north	s = south
	bool isAlive = true;
	list<int>::iterator iter;

	while (isAlive)
	{
		Sleep(300);

		// check for player input
		if (_kbhit()) 
		{
			char input = _getch();
			if	 ((input == 'w' && direction != 's')
				||(input == 'a' && direction != 'd')
				||(input == 's' && direction != 'w')
				||(input == 'd' && direction != 'a'))
					direction = input;
			if (input == 'q')
				isAlive = false;
		}

		//update snake
		//board[snake.front() / SIZEX][snake.front() % SIZEX] = 'b';
		// check for wall collision and advance snake
		switch (direction) 
		{
		case 'd':
			if (snake.front() % SIZEX + 1  == SIZEX)
			{
				isAlive = false;
				break;
			}
			snake.push_front(snake.front() + 1);
			break;

		case 'a':
			if (snake.front() % SIZEX - 1 == -1)
			{
				isAlive = false;
				break;
			}
			snake.push_front(snake.front() - 1);
			break;

		case 's':
			if (snake.front() + SIZEX >= SIZEX*SIZEX-1)
			{
				isAlive = false;
				break;
			}
			snake.push_front(snake.front() + SIZEX);
			break;

		case 'w':
			if (snake.front() - SIZEX <= -1)
			{
				isAlive = false;
				break;
			}
			snake.push_front(snake.front() - SIZEX);
			break; 
		}
		if (snake.front() > SIZEX * SIZEX - 1 || snake.front() < 0)
			throw ("out of bounds");

		if (isAlive)
		{
			board[*++snake.begin() / SIZEX][*++snake.begin() % SIZEX] = 'b';
			board[snake.back() / SIZEX][snake.back() % SIZEX] = ' ';
			board[snake.front() / SIZEX][snake.front() % SIZEX] = 'v';
			snake.pop_back();
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

		// print board
		set_cursor(0,0);
		for (int i = 0; i < SIZEX + 2; i++)
			cout << "x ";
		cout << endl;
		for (int i = 0; i < SIZEX; i++)
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

	cout << endl << "Game Over" << endl;

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