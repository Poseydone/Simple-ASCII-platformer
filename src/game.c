#include "../include/game.h"
#include <windows.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define FRICTION 0.99f
#define GRAVITY 2.0f
#define BOUNCE 0.5f
#define MAX_PLAYER_VELOCITY 20.0f
#define PLAYER_SPEED 10.0f
#define PLAYER_JUMP_SPEED 15.0f

//GLOBAL VARIABLES///////////////////////////////
/*
	A canvas is a bidimensional array that reprents the screen 
	and stores the symbols that must be drawn to represent a certain pixel.
*/
char** canvas;

/*
	A player is a bunch of variables, like position and velocity 
	that is drawn onto the canvas at each given frame.
*/
Rect player; //Not yet initialized



/////////////////////////////////////////////////


//FUNCTIONS//////////////////////////////////////
void ManageInput(bool* isRunning);
void InitCanvas(char** canvas);
void FreeCanvasMemory(char** canvas);
void DrawCanvas(char** canvas);
DWORD CalculateFrameTime(DWORD start_time, DWORD end_time);
void ShowStats(Rect player);
void InitPlayer(Rect* player);
void UpdatePlayer(Rect* player, float delta_time);

/////////////////////////////////////////////////


/*
	THE MAIN LOOP IS THE CORE OF THE GAME
*/
int GameLoop(void)
{
//MEMORY ALLOCATION
	////////////////////////////////////
	canvas = (char**)malloc(HEIGHT * sizeof(char*)); //Allocate first element as a list of pointers
	for(int i = 0; i < HEIGHT; i++)
	{
		canvas[i] = (char*)malloc(WIDTH * sizeof(char)); //Allocate each pointer as a list of char
	}
	////////////////////////////////////

//OBJECTS INITIALIZATION
	//All the following objects must be initialized before the loop starts
	////////////////////////////////////
	InitCanvas(canvas);
	InitPlayer(&player);
	////////////////////////////////////

	DWORD past_time = GetTickCount(); //Take the time before the first frame starts



//LOOP START
		////////////////////////////////////
		bool isRunning = true; //This variable cannot be global
		while(isRunning)
		{
			//Update the timer at the beginning of the new frame
			DWORD start_time = GetTickCount(); //Works in milliseconds

			//Clear screen
			ClearScreen();

			//Read Input
			ManageInput(&isRunning);

			//Update delta time
				DWORD current_time = GetTickCount(); //Time of the current frame
				float delta_time = ((float)current_time - (float)past_time) / 1000; //Delta time is the difference between current frame and previuos frame (/1000 means in seconds)
				past_time = current_time; //Update the time for the next frame
			///////////////////

			//Update
			InitCanvas(canvas); //Draws the level onto the canvas too
			UpdatePlayer(&player, delta_time);
			
			//Assign the player position on the canvas
			canvas[(int)player.y][(int)player.x] = '0';


			//Draw
			ShowStats(player);
			DrawCanvas(canvas);

			DWORD end_time = GetTickCount();
			DWORD time_to_wait = 0;
			time_to_wait = CalculateFrameTime(start_time, end_time);
			
			//Wait
			Sleep(time_to_wait);
		}
		////////////////////////////////////

		
		FreeCanvasMemory(canvas); //Free the canvas before returning to main
	}


//OTHER FUNCTIONS////////////////////////////////////

/*
	Name:ClearScreen
	Return:void
	Params:void

	Clears the screen calling the cls command for the cmd prompt.
*/
void ClearScreen(void)
{
	system("cls");
}

/*
	Name:ManageInput
	Return:void
	Param:bool

	This function manages the user input and converts it into an action.
	The only parameter it takes is a boolean value that if the ESC button is pressed
	sets the value to false. Usedo to quit the menu/application.
*/
void ManageInput(bool* isRunning)
{
	char key;
	while(kbhit())
	{			
		key = getch();

		switch(key)
		{
			case 'd':
			case 'D':
				player.vx += PLAYER_SPEED;
				if(player.vx >= MAX_PLAYER_VELOCITY)
					player.vx = MAX_PLAYER_VELOCITY;
				break;
			case 'a':
			case 'A':
				player.vx -= PLAYER_JUMP_SPEED;
				if(player.vx <= -MAX_PLAYER_VELOCITY)
					player.vx = -MAX_PLAYER_VELOCITY;
				break;
			case 32:
				if(player.isOnGround)	
				{
					player.vy -= PLAYER_JUMP_SPEED;
					player.isOnGround = false;
				}
				break;

			case 27:
				*isRunning = false;
		}
	}
}

/*
	Name:InitCanvas
	Return:void
	Param:char**

	This function takes a char** that represents the canvas and fills up the matrix with
	whathever the background should look like. '.' is used for debugging purposes.
*/
void InitCanvas(char** canvas)
{

	const char* level[15] =
	{
	    "--------------------------------------------------",
	    "|                                                |",
	    "|                                                |",
	    "|                                                |",
	    "|                                                |",
	    "|                       ????                     |",
	    "|                                                |",
	    "|               ####                             |",
	    "|                                                |",
	    "|         #######                                |",
	    "|                                                |",
	    "|                  ###           #               |",
	    "|                                #   #           |",
	    "|                      -------   #   #   #       |",
	    "--------------------------------------------------"
	};



	//Fill up the canvas with '.'
	for(int y = 0; y < HEIGHT; y++)
	{
		for(int x = 0; x < WIDTH; x++)
		{
			canvas[y][x] = level[y][x];
		}
	}
}

/*
	Name:InitPlayer
	Return:void
	Param:Rect*

	This function initialize all the player variables to a idle state.
*/
void InitPlayer(Rect* player)
{
	player->x = SPAWN_X;
	player->y = SPAWN_Y;
	player->vx = 0;
	player->vy = 0;
	player->isOnGround = false;
}

/*
	Name:FreeCanvasMemory
	Return:void
	Param:char**

	The function is used to release the memory allocated for the canvas.
	It frees each column, then the row.
*/
void FreeCanvasMemory(char** canvas)
{
	for(int i = 0; i < HEIGHT; i++)
	{
		free(canvas[i]);
	}

	free(canvas);
}

/*
	Name:DrawCanvas
	Return:void
	Param:char**

	Read the canvas and print it on the console.
*/
void DrawCanvas(char** canvas)
{
	for(int y = 0; y < HEIGHT; y++)
	{
		for(int x = 0; x < WIDTH; x++)
		{
			printf("%c", canvas[y][x]);
		}
		printf("\n");
	}
}	

/*
	Name:CalculateFrameTime
	Return:DWORD
	Param:DWORD, DWORD

	Calculate the time the given frame took to render and
	the amount of time to wait before the next frame starts
	to keep a constant frame-rate.
	The parameters are the time at the start of the frame and the time at the end of the frame.	
*/
DWORD CalculateFrameTime(DWORD start_time, DWORD end_time)
{
	DWORD time_to_wait = 0;
	DWORD current_frame_elapsed_time = end_time - start_time;
	if(current_frame_elapsed_time < FRAME_DURATION)
	{
		time_to_wait = FRAME_DURATION - current_frame_elapsed_time;
	}	
	return time_to_wait;
}

/*
	Name:ShowStats
	Return:void
	Param:void

	Show some debug statistics in the corner of the screen.
*/
void ShowStats(Rect player)
{
	printf(
		"FPS: %d\n"
		"Player X: %f, Player Y: %f\n"
		"Player VX: %f, Player VY: %f\n"
		"isOnGround: %d\n"
		"Gravity: %f\n",
		FPS,
		player.x, player.y,
		player.vx, player.vy,
		player.isOnGround,
		GRAVITY);
}

/*
	Name:UpdatePlayer
	Return:void
	Param:Rect*

	This function takes the player rectangle and applies forces and collision checking to it.
*/
void UpdatePlayer(Rect* ply, float delta_time)
{
	//Add friction
	/*
		Friction is a force applied to the player that curbs his horizontal movement.
		To simulate it I need to apply a force in the opposite direction of the movement.
	*/
	if(ply->vx > 0)
	{
		ply->vx -= FRICTION;
		if(ply->vx <= 0.2)
			ply->vx = 0;
	}
	else if(ply->vx < 0)
	{
		ply->vx += FRICTION;
		if(ply->vx >= -0.2)
			ply->vx = 0;
	}

	//Add gravity
	/*
		Gravity is a force applied to the player that drags him down on the floor constantly.
		To simulate it I need to add a downward acceleration each frame.
		If the player is on the ground I can stop applying it.
	*/
	if(ply->isOnGround == false)
	{
		ply->vy += GRAVITY;
	}

//COLLISIONS
	////////////////////////////////////

	float next_y = ply->y + ply->vy * delta_time;
	float next_x = ply->x + ply->vx * delta_time;

	//Clamp the position if it lands out of bounds
	if(next_y >= HEIGHT - 1)
	{
		next_y = HEIGHT - 2;
		ply->vy = 0;
		ply->isOnGround = true;
	}
	else if(next_y < 1)
	{
		next_y = 1;
		ply->vy = 0;
	}

	if(next_x >= WIDTH - 1)
	{
		next_x = WIDTH - 2;
		ply->vx = 0;
	}
	else if(next_x < 1)
	{
		next_x = 1;
		ply->vx = 0;
	}

	//Moving DOWN
	if(ply->vy > 0)
	{
		for(int curr_tile = (int)ply->y; curr_tile <= (int)next_y; curr_tile++)
		{
			//Check if current tile is walkable
			if(canvas[curr_tile][(int)ply->x] != ' ')
			{
				next_y = curr_tile - 1;
				ply->vy = 0;
				ply->isOnGround = true;
				break;
			}
		}
	}

	//Moving UP
	if(ply->vy < 0)
	{
		for(int curr_tile = (int)ply->y; curr_tile >= (int)next_y; curr_tile--)
		{
			//Check if current tile is walkable
			if(canvas[curr_tile][(int)ply->x] != ' ')
			{
				next_y = curr_tile + 1;
				ply->vy = 0;
				break;
			}
		}
	}

	//Moving RIGHT
	if(ply->vx > 0)
	{
		for(int curr_tile = (int)ply->x; curr_tile <= (int)next_x; curr_tile++)
		{
			//Check if current tile is walkable
			if(canvas[(int)ply->y][curr_tile] != ' ')
			{
				next_x = curr_tile - 1;
				ply->vx = 0;
				break;
			}
		}
	}

	//Moving LEFT
	if(ply->vx < 0)
	{
		for(int curr_tile = (int)ply->x; curr_tile >= (int)next_x; curr_tile--)
		{
			//Check if current tile is walkable
			if(canvas[(int)ply->y][curr_tile] != ' ')
			{
				next_x = curr_tile + 1;
				ply->vx = 0;
				break;
			}
		}
	}


	//Bottom collision gets updated only when falling, should happen every frame.
	if(canvas[(int)ply->y + 1][(int)ply->x] == ' ')
	{
		ply->isOnGround = false;
	}

	////////////////////////////////////
	
	
	ply->y = next_y;
	ply->x 	= next_x;
}
