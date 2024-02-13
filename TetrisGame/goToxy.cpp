#include <windows.h> 
#include <process.h> 
#include <iostream>
using namespace std;

//the function fo to x y location on screen
void gotoxy(int x, int y)
{
	HANDLE hConsoleOutput;
	COORD dwCursorPosition;
	cout.flush();
	dwCursorPosition.X = x;
	dwCursorPosition.Y = y;
	hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(hConsoleOutput, dwCursorPosition);
}

//The function clear the screen
void clrscr()
{
	system("cls");
}