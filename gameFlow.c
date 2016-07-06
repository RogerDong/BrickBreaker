
#include "cFile.h"

//Use this function to start the ganme.
void baseGame()
{
    //Trick and brickNum for passing last level.
    //quit is for the game progre.
    int trick = 0,brickNum = 0;
    int quit=0;
    while( quit == 0 )
    {
        //restart the game
        if( text.restart == 1 )
        {
            init();
            if( Mix_PlayMusic( music, -1 ) == -1 )
            {
                return ;
            }
            brickNum = 0;
            trick = 0;
        }
        //Memorize the start time
        timeStart();
        //Get and do the event by the player.
        while(SDL_PollEvent( &event ))
        {
            if( event.type == SDL_QUIT )
            {
                quit = 1;
            }
            moveJudge();
            if( text.level == 3 )
            {
                if( mouseEvent() == 1 )
                {
                    trick = 1;
                }
            }
        }
        //If touch the button trick, passing the last level.
        if( trick > 0 && brickNum < 100 )
        {
            if( brick[brickNum].brick != 1 )
            {
                brick[brickNum].life = 0;
            }
            brickNum ++ ;
        }
        gameProgress();
        moveAll();
        //Control the frame rate.
        if( getTime() < 1000.0 / 60 )
        {
            SDL_Delay( ( 1000.0 / 60 ) - getTime() );
        }
    }
}


//Move all the things such as ball, paddle and so on in one function.
void moveAll()
{
    int i = 0;
    moveP();
    moveBall( &ball );
    if( brickCollision( &ball ) == 1 )
    {
        if( ball.explode == 0 )
        {
            Mix_PlayChannel( -1, collision, 0 );
        }
        else if( ball.explode == 1 )
        {
            Mix_PlayChannel( -1, explode, 0 );
            ball.explode = 0;
        }
    }
    for( i=0; i<3; i++)
    {
        if( cloneBall[i].life > 0 )
        {
            moveBall( &cloneBall[i] );
            if( brickCollision( &cloneBall[i] ) == 1 )
            {
                Mix_PlayChannel( -1, collision, 0 );
            }
        }
    }
}

//During the game progress, we have many functions. We link these in one function together.
void gameProgress()
{
    if( bonus.bonusMode != -1 )
    {
        bonusEffect();
    }
    if( winLevel() == 1 && text.level < 3 )
    {
        enterNext();
    }
    if( ball.life == 0 )
    {
        stopGame(1);
    }
    else if( text.level == 3 && winLevel() == 1)
    {
        stopGame(2);
    }
    else
    {
        blitPicture();
    }

    if( timer.paused == 1 )
    {

        pause();
        bonus.bonusMode = -1;
        timer.paused = 0;
    }
}

//Initialize the many things before the  game.
void startGame()
{
    //Initialize tha game
    if(init()==0)
    {
        return ;
    }
    //Start play the music
    if( Mix_PlayMusic( music, -1 ) == -1 )
    {
        return ;
    }
}
