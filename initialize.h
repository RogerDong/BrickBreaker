

#ifndef initialize_h
#define initialize_h
void bricksInit();
int init();
SDL_Surface *loadImage( const char *fileName, int type );
int loadFiles();
void applySurface( int x, int y, SDL_Surface* source, SDL_Surface* dest, SDL_Rect* clip );
void applyPaddle();

void freeGame();
void blitPicture();
void paddleInit();
void ballInit();
void bonusInit();
void textInit();
void ballCenter();
void briksLevel1();
void showBricks();

void randomBonus( Brick givenBrick );
void applyBonus();
void fallBonus( Brick givenBrick );
void brickLevel();



void print(int x,int y,const char *str,int color,int fontSize);
void showFont();
void saveGame();
void loadGame();

void buttonInit();



#endif
