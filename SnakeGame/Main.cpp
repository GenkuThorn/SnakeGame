#include <iostream>
#include <list>
#include <iterator>
#include <windows.h>
#include <conio.h>

#define SIZEX 20
#define SIZEY 20 //not currently used
#define STARTSIZE 3
using namespace std;

void set_cursor(int, int);
int findRandomEmptySpace(char** board);

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
	snake.push_front(3 + SIZEX / 2 * SIZEX);
	for (int i = 0; i < STARTSIZE; i++)
	{
		board[snake.front() / SIZEX][snake.front() % SIZEX] = '=';
		snake.push_front(snake.front() + 1);
	}
	board[snake.front() / SIZEX][snake.front() % SIZEX] = '>';

	char direction = 'd'; // d = east	a = west	w = north	s = south
	bool isAlive = true;
	list<int>::iterator iter;
	int score = 0;
	srand(time(0));
	bool isPaused = false;

	//apple generation
	int apple = findRandomEmptySpace(board);
	board[apple / SIZEX][apple % SIZEX] = 'a';

	// portal generation
	int portalX, portalY;
	bool genDone = false;
	while (!genDone)
	{
		portalX = findRandomEmptySpace(board);
		portalY = findRandomEmptySpace(board);
		if (portalX % SIZEX != 0 &&			// no portal at wall allowed
			portalX % SIZEX != SIZEX - 1 &&
			portalX / SIZEX != 0 &&
			portalX / SIZEX != SIZEX - 1 &&
			portalY % SIZEX != 0 &&
			portalY % SIZEX != SIZEX - 1 &&
			portalY / SIZEX != 0 &&
			portalY / SIZEX != SIZEX - 1)
			genDone = true;
	}
	board[portalX / SIZEX][portalX % SIZEX] = 'O';
	board[portalY / SIZEX][portalY % SIZEX] = 'O';

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
			if (snake.front() % SIZEX + 1  == SIZEX)
			{
				isAlive = false;
				break;
			}
			if (snake.front() + 1 == portalY)
			{
				snake.push_front(portalX + 1);
				break;
			}
			if (snake.front() + 1 == portalX)
			{
				snake.push_front(portalY + 1);
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
			if (snake.front() - 1 == portalY)
			{
				snake.push_front(portalX - 1);
				break;
			}
			if (snake.front() - 1 == portalX)
			{
				snake.push_front(portalY - 1);
				break;
			}
			snake.push_front(snake.front() - 1);
			break;

		case 's':
			if (snake.front() + SIZEX >= SIZEX * SIZEX)
			{
				isAlive = false;
				break;
			}
			if (snake.front() + SIZEX == portalY)
			{
				snake.push_front(portalX + SIZEX);
				break;
			}
			if (snake.front() + SIZEX == portalX)
			{
				snake.push_front(portalY + SIZEX);
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
			if (snake.front() - SIZEX == portalY)
			{
				snake.push_front(portalX - SIZEX);
				break;
			}
			if (snake.front() - SIZEX == portalX)
			{
				snake.push_front(portalY - SIZEX);
				break;
			}
			snake.push_front(snake.front() - SIZEX);
			break;
		}
		if (snake.front() > SIZEX * SIZEX - 1 || snake.front() < 0)
			throw ("out of bounds");

		//advance snake
		if (isAlive)
		{	
			if (snake.front() != apple)
			{
				board[snake.back() / SIZEX][snake.back() % SIZEX] = ' ';
				snake.pop_back();
			}
			board[*++snake.begin() / SIZEX][*++snake.begin() % SIZEX] = '=';
			switch (direction)
			{
			case 'a':
				board[snake.front() / SIZEX][snake.front() % SIZEX] = '<';
				break;
			case 'd':
				board[snake.front() / SIZEX][snake.front() % SIZEX] = '>';
				break;
			case 'w':
				board[snake.front() / SIZEX][snake.front() % SIZEX] = '^';
				break;
			case 's':
				board[snake.front() / SIZEX][snake.front() % SIZEX] = 'v';
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
		board[apple / SIZEX][apple % SIZEX] = 'a';

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

	cout << endl << "Game Over: " << score << endl;

	Sleep(1000);

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

int findRandomEmptySpace(char** board)
{
	int var;
	bool valid = false;
	while (!valid)
	{
		var = rand() % (SIZEX * SIZEX);
		if (board[var / SIZEX][var % SIZEX] == ' ')
			valid = true;
	}
	return var;
}

//end of file