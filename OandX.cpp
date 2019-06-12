#include "pch.h"
#include <iostream>
#include <conio.h>
#include <Windows.h>
#include <cstdlib>
#include <ctime>
#include <windows.h>

using namespace std;
#define LEFT 72
#define RIGHT 80
#define UP 75
#define DOWN 77
#define INSERT_X 120
#define INSERT_O 111
#define CLEAR_SIGN 13
void SetCursorPosition(int x, int y);
void clearSign(int x, int y, char** board);
bool insertO(int x, int y, char** board);
bool insertX(int x, int y, char** board);
char** createBoard(int XYsize);
void displayBoard(int XYsize, char **board);
int dynamicDisplay(int XYsize, char **board, short player, unsigned int depth,int pts);
void blinkingBox(int x, int y, char **board, int XYsize);
bool isWin(char ** board, char player, int size);
bool isDraw(char ** board, int size);
int minMax(char ** board, char player, int size, unsigned int depth, unsigned int maxDepth, int pts);
void AI(char ** board, char symbol, int size, unsigned int maxDepth,int pts);

int main()
{
	//srand(time(NULL));
	int size, XYsize, pts, symbol;
	unsigned int depth;
	char** board;
	int stats[] = { 0,0,0,0};
	
	char choose;
	do {
		cout << "Podaj rozmiar planszy: ";
		cin >> size;							//DODAÆ IDIOTOODPORNOŒÆ
		XYsize = 5 * size + 3;
		pts = size * 2 + 2;

		cout << "Wybierz symbol:\n[1] X\n[2] O(rozpoczyna)\n";
		cin >> symbol;
		cout << "Podaj poziom trudnosci: ";
		cin >> depth;
		system("CLS");
		board = createBoard(XYsize);
		int win = dynamicDisplay(XYsize, board, symbol, ++depth, pts);
		if (win == 1) {
			cout << "Wygrales!\n";
			stats[2]++;
		}
		else if (win) {
			cout << "Przegrales\n";
			stats[1]++;
		}
		else
		{
			cout << "Remis\n";
			stats[3]++;
		}
		cout << endl;
		stats[0]++;
		cout << "Statystyki:" << endl;
		cout << "Rozegrane gry: " << stats[0] << " Wygrane AI: " << stats[1] <<" Wygrane Gracza: " << stats[2] <<" Remisy: " << stats[3] << endl;
		cout<<endl;
		cout << "Jeszcze raz?(t-tak lub dowolna zeby zakonczyc)" << endl;
		cin >> choose;
		if (choose != 't')
		{
			system("pause");
			return 0;
		}
		system("CLS");
		
		
	} while (choose = 't');

	system("pause");
	return 0;
}

char** createBoard(int XYsize)
{
	char **board = new char*[XYsize];
	for (int i = 0; i < XYsize; ++i)
		board[i] = new char[XYsize];
	for (int i = 1; i < XYsize-1; ++i)
		for (int j = 1; j <= XYsize-1; ++j)
		{
			if (i % 5 > 0 && i % 5 <= 1)
				board[i][j] = '-';
			else
			{
				if (j % 5 > 0 && j % 5 <= 1)
					board[i][j] = '.';
				else
					board[i][j] = ' ';
			}
		}
	return board;
}

void displayBoard(int XYsize, char **board)
{
	for (int i = 1; i < XYsize-1; ++i)
	{
		for (int j = 1; j < XYsize-1; ++j)
			cout << board[i][j];
		cout << endl;
	}
}

int dynamicDisplay(int XYsize, char **board, short player, unsigned int depth,int pts)
{
	bool isMove = false;
	int key = 0, x = 2, y = 2, moves = 0;
	char computerSymbol = player == 1 ? 'O' : 'X';
	if (computerSymbol == 'O' )
	{
		AI(board, computerSymbol, XYsize, depth, pts);
		moves++;
		if (moves > 2)
		{
			if (isWin(board, computerSymbol, XYsize))
			{
				blinkingBox(x, y, board, XYsize);
				return 2;
			}
			if (isDraw(board, XYsize))
			{
				blinkingBox(x, y, board, XYsize);
				return 0;
			}
		}
	}


	for (;;)
	{

		
			blinkingBox(x, y, board, XYsize);
			//if (_kbhit())
			key = _getch();
			switch (key)
			{
			case UP:
			{
				if (y > 6)
					y -= 5;
				break;
			}
			case DOWN:
			{
				if (y < XYsize - 6)
					y += 5;
				break;
			}
			case LEFT:
			{
				if (x > 6)
					x -= 5;
				break;
			}
			case RIGHT:
			{
				if (x < XYsize - 6)
					x += 5;
				break;
			}
			case INSERT_X:
			{
				if (player == 1)
				{
					isMove = insertX(x, y, board);

					//isMove = true;
				}
				break;
			}
			case INSERT_O:
			{
				if (player == 2)
				{
					isMove = insertO(x, y, board);
					//isMove = true;
				}
				break;
			}case CLEAR_SIGN:
			{
				break;
			}
			default:
				break;
			}
			key = 0;
			if (!isMove)
				continue;

			isMove = false;
			++moves;
			if (moves > 2)
			{
				if (isWin(board, player == 1 ? 'X' : 'O', XYsize))
				{
					blinkingBox(x, y, board, XYsize);
					return 1;
				}
				if (isDraw(board, XYsize))
				{
					blinkingBox(x, y, board, XYsize);
					return 0;
				}
			}
			AI(board, computerSymbol, XYsize, depth, pts);
			if (moves > 2)
			{
				if (isWin(board, computerSymbol, XYsize))
				{
					blinkingBox(x, y, board, XYsize);
					return 2;
				}
				if (isDraw(board, XYsize))
				{
					blinkingBox(x, y, board, XYsize);
					return 0;
				}
			}
		
	



			
	}
}

void blinkingBox(int x, int y, char** board, int XYsize)
{
	/*for (int i = y, s = y + 5; i < s; ++i)
		board[x][i] = 'x';
	x += 4;
	for (int i = y, s = y + 5; i < s; ++i)
		board[x][i] = 'x';
	x -= 3;
	for (int i = x, s = x + 3; i < s; ++i)
		board[i][y] = 'x';
	y += 4;
	for (int i = x, s = x + 3; i < s; ++i)
		board[i][y] = 'x';
	system("CLS");
	*/
	SetCursorPosition(0, 0);
	board[x + 1][y + 2] = board[x + 3][y + 2] = '-';
	board[x + 2][y + 1] = board[x + 2][y + 3] = '|';
		displayBoard(XYsize, board);
		board[x + 1][y + 2] = board[x + 3][y + 2] = ' ';
		board[x + 2][y + 1] = board[x + 2][y + 3] = ' ';
	
	
	
	
	
	/*for (int i = x, s = x + 3; i < s; ++i)
		board[i][y] = '|';
	y -= 4;
	for (int i = x, s = x + 3; i < s; ++i)
		board[i][y] = '|';
	x += 3;
	for (int i = y, s = y + 5; i < s; ++i)
		board[x][i] = '=';
	x -= 4;
	for (int i = y, s = y + 5; i < s; ++i)
		board[x][i] = '=';
	system("CLS");
	//Sleep(42);
	displayBoard(XYsize, board);*/
}

bool isWin(char ** board, char player, int size)
{
	size -= 3;
	int score = 0;
	for (int i = 4; i < size; i += 5) //
		if (board[i][i] == player)
			++score;
	if (score == size / 5)
		return true;
	score = 0;
	for (int i = 4, j = size - 1; i < size; i += 5, j -= 5) //
		if (board[i][j] == player)
			++score;
	if (score == size / 5)
		return true;
	for (int i = 4; i < size; i += 5) //
	{
		score = 0;
		for (int j = 4; j < size; j += 5)
			if (board[i][j] == player)
				++score;
		if (score == size / 5)
			return true;
	}
	for (int i = 4; i < size; i += 5) //
	{
		score = 0;
		for (int j = 4; j < size; j += 5)
			if (board[j][i] == player)
				++score;
		if (score == size / 5)
			return true;
	}
	return false;
}

bool isDraw(char ** board, int size)
{
	int score = 0;
	size -= 3;
	for (int i = 4; i < size; i += 5)
		for (int j = 4; j < size; j += 5)
			if (board[i][j] != ' ')
				++score;
	if (score == (size*size) / 25)
		return true;
	return false;
}

int minMax(char ** board, char player, int size, unsigned int depth, unsigned int maxDepth,int pts)
{
	if (depth > maxDepth)
		return 0;
	if (isWin(board, player, size + 3))
		return player == 'X' ? 1 : -1;
	if (isDraw(board, size + 3))
		return 0;
	player = player == 'X' ? 'O' : 'X';
	int m, mmx = player == 'O' ? pts : -pts;
	for (int i = 4; i < size; i += 5)
		for (int j = 4; j < size; j += 5)
			if (board[i][j] == ' ')
			{
				board[i][j] = player;
				m = minMax(board, player, size, ++depth, maxDepth,pts);
				board[i][j] = ' ';
				if ((player == 'O' && m < mmx) || (player == 'X' && m > mmx))
					mmx = m;
			}
	return mmx;
}

void AI(char ** board, char symbol, int size, unsigned int maxDepth,int pts)
{
	int x = 4, y = 4, m = 0, mmx = symbol == 'O' ? pts : -pts;
		size -= 3;
		for (int i = 4; i < size; i += 5)
			for (int j = 4; j < size; j += 5)
				if (board[i][j] == ' ')
				{
					board[i][j] = symbol;
					m = minMax(board, symbol, size, 0, maxDepth,pts);
					board[i][j] = ' ';
					if ((symbol == 'O' && m < mmx) || (symbol == 'X' && m > mmx))
					{
						mmx = m;
						x = i;
						y = j;
					}
				};

	if (symbol == 'X')
		insertX(x - 2, y - 2, board);
	else
		insertO(x - 2, y - 2, board);
}

bool insertX(int x, int y, char** board)
{
	if (board[x + 2][y + 2] == ' ')
	{
		board[x + 2][y + 2] = 'X';
		//board[x + 1][y + 1] = board[x + 3][y + 3] = '\\';
		//board[x + 1][y + 3] = board[x + 3][y + 1] = '/';
		//board[x + 1][y + 2] = board[x + 3][y + 2] = board[x + 2][y + 1] = board[x + 2][y + 3] = ' ';
		return true;
	}
	return false;
}

bool insertO(int x, int y, char** board)
{
	if (board[x + 2][y + 2] == ' ')
	{
		//board[x + 1][y + 1] = board[x + 1][y + 3] = board[x + 3][y + 1] = board[x + 3][y + 3] = ' ';
		//board[x + 1][y + 2] = board[x + 3][y + 2] = '-';
		//board[x + 2][y + 1] = board[x + 2][y + 3] = '|';
		board[x + 2][y + 2] = 'O';
		return true;
	}
	return false;
}

void clearSign(int x, int y, char** board)
{
	for (int i = x + 1, s = x + 4; i < s; ++i)
		for (int j = y + 1, t = y + 4; j < t; ++j)
			board[i][j] = ' ';
}

void SetCursorPosition(int x, int y)
{
	HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos = { x, y };
	SetConsoleCursorPosition(output, pos);



}




/*
int minimax(bool flag)// The minimax function
{

	int max_val = -1000, min_val = 1000;
	int i, j, value = 1;
	if (cpu_won() == 1)
	{
		return 10;
	}
	else if (user_won() == 1)
	{
		return -10;
	}
	else if (isFull() == 1)
	{
		return 0;
	}
	int score[9] = { 1,1,1,1,1,1,1,1,1 };//if score[i]=1 then it is empty

	for (i = 0; i < 9; i++)
	{
		if (board[i] == '*')
		{
			if (min_val > max_val) // reverse of pruning condition.....
			{
				if (flag == true)
				{
					board[i] = 'X';
					value = minimax(false);
				}
				else
				{
					board[i] = 'O';
					value = minimax(true);
				}
				board[i] = '*';
				score[i] = value;
			}
		}
	}

	if (flag == true)
	{
		max_val = -1000;
		for (j = 0; j < 9; j++)
		{
			if (score[j] > max_val && score[j] != 1)
			{
				max_val = score[j];
				index1 = j;
			}
		}
		return max_val;
	}
	if (flag == false)
	{
		min_val = 1000;
		for (j = 0; j < 9; j++)
		{
			if (score[j] < min_val && score[j] != 1)
			{
				min_val = score[j];
				index1 = j;
			}
		}
		return min_val;
	}
}
*/