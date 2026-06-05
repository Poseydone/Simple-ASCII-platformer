#ifndef GAME_H
#define GAME_H
#endif

#include <stdbool.h>

#define WIDTH 50
#define HEIGHT 15
#define FPS	60
#define FRAME_DURATION 1000/FPS
#define SPAWN_X 1
#define SPAWN_Y 8


typedef struct __rectangle
{
	float x; //Position on the X
	float y; //Position on the Y
	float vx; //Horizontal velocity
	float vy; //Vertical velocity
	bool isOnGround; //Flag to know if the player is touching the ground
}Rect;


int GameLoop(void);
void ClearScreen(void);
