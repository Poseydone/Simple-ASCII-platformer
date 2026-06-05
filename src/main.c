/*
	This is the entry point of the program.
	The main function calls the gameloop.
*/

#include <stdio.h>
#include <windows.h>
#include <stdbool.h>
#include "../include/game.h"

void setCursorVisibility(bool visible);

int main(void)
{
	setCursorVisibility(false);

	GameLoop();

	setCursorVisibility(true);

	ClearScreen();
	printf("\nBYE BYE\n");
	Sleep(500);
	ClearScreen();
}


void setCursorVisibility(bool visible)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cursor;

	GetConsoleCursorInfo(hConsole, &cursor);

	cursor.bVisible = visible;
	
	SetConsoleCursorInfo(hConsole, &cursor);
}