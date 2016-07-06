

#ifndef process_h
#define process_h
void moveP();
void moveJudge();
void moveBall();
void timeStart();
int getTime();
int paddleCollision( Ball *givenBall );
int brickCollision( Ball *givenBall );
void scorePlus( Brick givenBrick );
int winLevel();
void stopGame( int type );
void lifeMinus();

double distance( int x1, int y1, int x2, int y2 );

void nearestPoint( int *coordinate, Brick givenBrick, Ball *givenBall );

void moveBall( Ball *givenBall );
int pause();
void nextLevel();
int mouseEvent();
void enterNext();


#endif // process_h
