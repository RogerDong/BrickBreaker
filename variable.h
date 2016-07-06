
/*To declare create global variables to be used in every files.*/
#ifndef variable_h
#define variable_h
//To declare structures.
/*In these strctures, we declare many variables such as width and height that we can use in the other c files*/
typedef struct paddle Paddle;
struct paddle
{
    SDL_Surface *paddle;
    SDL_Surface *increase;
    int stick;
    int stickVel;
    int paddleSpeed;
    int x;
    int y;
    int vel;
    int paddleWidth;
    int paddleHeight;
    int length;
};
typedef struct
{
    SDL_Surface *ballPic;
    SDL_Rect clip[4];
    int life;
    int x;
    int y;
    int r;
    int centerX;
    int centerY;
    int xVel;
    int yVel;
    int ballSpeed;
    int explode;
    int animate;
    int ballWidth;
    int ballHeight;
}Ball;
typedef struct
{
    int startTime;
    int pausedTime;

    int paused;
    int started;
}Timer;

typedef struct
{
    TTF_Font *font;
    int level;
    int score;
    int restart;

}Text;



typedef struct
{
    int brick;
    int life;
    SDL_Rect clip;
    int x;
    int y;
    int alpha;

}Brick;

typedef struct
{
    SDL_Surface *bonus;
    int bonusMode;
    int x;
    int y;
    int yVel;
    SDL_Rect clip[8];
    int bonusWidth;
    int bonusHeight;

}Bonus;

typedef struct
{
    SDL_Surface *button;
    SDL_Rect clip[2];
    int x;
    int y;
    int w;
    int h;
    int num;
}Button;

//To declare the variables that can be used by the whole c files
#define PI 3.14
extern const int SCREEN_BORDER;
extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;
extern const int SCREEN_BPP;
extern const int BRICK_WIDTH;
extern const int BRICK_HEIGHT;
extern const int SCREEN_LEFT;
extern const int SCREEN_RIGHT;
extern const int SCREEN_UP;

//Declare the SDL surface
extern SDL_Surface *screen;
extern SDL_Surface *backGround;
extern SDL_Surface *endGame;
extern SDL_Surface *winGame;


extern SDL_Event event;
extern Paddle myPaddle;
extern Ball ball;
extern Ball cloneBall[3];
extern Timer timer;


extern SDL_Surface *brickPic;
extern SDL_Surface *brick2;
extern Brick brick[100];

extern SDL_Surface *message;
extern Text text;
extern Bonus bonus;

extern SDL_Color fontColor;


//Declare the SDL music for the game
extern Mix_Music *music;
extern Mix_Chunk *collision;
extern Mix_Chunk *endSound;
extern Mix_Chunk *explode;
extern Button button;

#endif // variable_h



