
#include "cFile.h"
//To create global variables.

const int SCREEN_WIDTH=800;
const int SCREEN_HEIGHT=600;
const int SCREEN_BPP=32;
const int BRICK_WIDTH = 52;
const int BRICK_HEIGHT = 25;
const int SCREEN_LEFT = 278;
const int SCREEN_RIGHT = 2;
const int SCREEN_UP = 40;


//Create the event variable
SDL_Event event;

//Create the SDL surface
SDL_Surface *backGround=NULL;
SDL_Surface *screen=NULL;
SDL_Surface *brickPic=NULL;
SDL_Surface *brick2=NULL;

SDL_Surface *endGame=NULL;
SDL_Surface *winGame=NULL;

SDL_Surface *message=NULL;

SDL_Color fontColor = { 0, 0, 0 };

//Create the SDL music for the game
Mix_Music *music = NULL;
Mix_Chunk *collision = NULL;
Mix_Chunk *endSound = NULL;
Mix_Chunk *explode = NULL;

//Creat 100 bricks ball, paddle and other things.
Brick brick[100];
Paddle myPaddle;
Ball ball;
Ball cloneBall[3];
Timer timer;
Text text;
Bonus bonus;

Button button;


