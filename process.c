#include "cFile.h"

//To move the paddle.
void moveP()
{
    myPaddle.x += myPaddle.vel;
    //If paddle get the sticky bonus, it will memorize the steps the paddle move
    if( myPaddle.stick > 0 )
    {
        myPaddle.stickVel = myPaddle.vel;
    }
    //If it is within the specified range,it can move.
    if(myPaddle.x < SCREEN_LEFT )
    {
        myPaddle.x = SCREEN_LEFT;
        if( myPaddle.stick > 0 )
        {
            myPaddle.stickVel = 0;
        }
    }
    else if( myPaddle.x+myPaddle.paddleWidth > SCREEN_WIDTH-SCREEN_RIGHT )
    {
        myPaddle.x = SCREEN_WIDTH - SCREEN_RIGHT - myPaddle.paddleWidth ;
        if( myPaddle.stick > 0 )
        {
            myPaddle.stickVel = 0;
        }
    }
}

//To move the ball.
void moveBall( Ball *givenBall )
{
    //If the stick is 2, the ball will stick to the paddle
    if( myPaddle.stick == 2 )
    {
        if( event.key.keysym.sym == SDLK_UP && event.type == SDL_KEYDOWN  )
        {
            paddleCollision( givenBall );
            //Recover stick to 1
            myPaddle.stick = 1;
            givenBall->x += givenBall->xVel;
            givenBall->y += givenBall->yVel;
        }
        else
        {
            stickball();
        }
    }
    //If it happens collision,we cannot change the position of x and y before judging.
    else if(paddleCollision( givenBall ))
    {
        givenBall->y = SCREEN_HEIGHT-myPaddle.paddleHeight*2-givenBall->ballPic->h;
        //If the stick is 1, when ball collide with paddle ,it will become 2 and stick to paddle.
        if( myPaddle.stick == 1 )
        {
            myPaddle.stick = 2;
        }
        else
        {
            givenBall->x += givenBall->xVel;
            givenBall->y += givenBall->yVel;
        }


    }
    //If it doesn't happen collision,we can change the position of x and y after judging.
    else
    {
        givenBall->x += givenBall->xVel;
        if(givenBall->x < SCREEN_LEFT )
        {
            givenBall->x = SCREEN_LEFT;
            givenBall->xVel = -givenBall->xVel;
            givenBall->x += givenBall->xVel;
        }
        else if( givenBall->x+givenBall->ballHeight > SCREEN_WIDTH-SCREEN_RIGHT )
        {
            givenBall->x = SCREEN_WIDTH-SCREEN_RIGHT-givenBall->ballHeight;

            givenBall->xVel = -givenBall->xVel;
            givenBall->x += givenBall->xVel;
        }
        givenBall->y += givenBall->yVel;
        if( givenBall->y < SCREEN_UP )
        {
            givenBall->y = SCREEN_UP;
            givenBall->yVel = -givenBall->yVel;
            givenBall->y += givenBall->yVel;
        }
        else if( givenBall->y > SCREEN_HEIGHT )
        {
            //If the ball go under the screen's bottom, it will start function lifeMinus().
            if( givenBall->life > 1 )
            {
                lifeMinus();
            }
            if( givenBall->life > 0 )
            {
                givenBall->life --;
            }
            //givenBall->yVel = -givenBall->yVel;

        }
    }
    ballCenter();

}

//To judge wheher the keyboard is pressed or not.
void moveJudge()
{
    //If it is pressed,give the paddle a speed.
    if( event.type == SDL_KEYDOWN )
    {

        switch( event.key.keysym.sym )
        {
        case SDLK_LEFT:
            myPaddle.vel -= myPaddle.paddleSpeed;
            break;
        case SDLK_RIGHT:
            myPaddle.vel += myPaddle.paddleSpeed;
            break;
        case SDLK_p:
            if( timer.paused == 0 )
            {
                timer.paused = 1;
            }
            else if( timer.paused == 1 )
            {
                timer.paused = 0;
            }
        default:
            break;
        }
    }
    //If it is released,change the speed.
    else if( event.type == SDL_KEYUP )
    {
        switch( event.key.keysym.sym )
        {
        case SDLK_LEFT:
            myPaddle.vel += myPaddle.paddleSpeed;
            break;
        case SDLK_RIGHT:
            myPaddle.vel -= myPaddle.paddleSpeed;
            break;
        default:
            break;
        }
    }
}


//Create a variable called touch to let other function konw that the bonus fall on paddle or out of screen.
//To check whether there exists collision between paddle and the ball.
int paddleCollision( Ball *givenBall )
{
    float angle = 0.0;
    //If there exists collision between paddle and the ball,to caculate the new angle and change the velocity in x and y.
    if(givenBall->y+ball.ballHeight>=myPaddle.y && givenBall->y+ball.ballHeight<=myPaddle.y+10 && givenBall->x+ball.ballHeight/2>=myPaddle.x && givenBall->x+ball.ballHeight/2<=myPaddle.x+myPaddle.paddleWidth)
    {
        //Calculate angle and change the speed
        if(givenBall->x+ball.ballHeight/2 <= myPaddle.paddleWidth/2+myPaddle.x)
        {
            angle = PI/2-PI/3/(myPaddle.paddleWidth/2)*(myPaddle.x+myPaddle.paddleWidth/2-(givenBall->x+ball.ballHeight/2));
            givenBall->xVel = -(int)(givenBall->ballSpeed*cos(angle));
            givenBall->yVel = -(int)(givenBall->ballSpeed*sin(angle));
        }
        else if(givenBall->x+ball.ballHeight/2 > myPaddle.paddleWidth/2+myPaddle.x)
        {
            angle = PI/2-PI/3/(myPaddle.paddleWidth/2)*(givenBall->x+ball.ballHeight/2-(myPaddle.x+myPaddle.paddleWidth/2));
            givenBall->xVel = (int)(givenBall->ballSpeed*cos(angle));
            givenBall->yVel = -(int)(givenBall->ballSpeed*sin(angle));
        }

        return 1;
    }
    return 0;
}

//Find the nearest point between ball and brick.
void nearestPoint( int *coordinate, Brick givenBrick, Ball *givenBall )
{
    if( givenBall->centerX < givenBrick.x )
    {
        coordinate[0] = givenBrick.x;
    }
    else if(  givenBall->centerX> givenBrick.x + givenBrick.clip.w )
    {
        coordinate[0] = givenBrick.x + givenBrick.clip.w;
    }
    else
    {
        coordinate[0] = givenBall->centerX;
    }
    if( givenBall->centerY < givenBrick.y)
    {
        coordinate[1] = givenBrick.y;
    }
    else if( givenBall->centerY> givenBrick.y + givenBrick.clip.h )
    {
        coordinate[1] = givenBrick.y + givenBrick.clip.h;
    }
    else
    {
        coordinate[1] = givenBall->centerY;
    }
}

//Check whether it exist the collision between brick and ball. If exist, change the speed and coordinate of the ball.
int brickCollision( Ball *givenBall )
{
    int coordinate[2] = {0};
    int i = 0;
    for(i=0; i<100; i++)
    {
        if( brick[i].life > 0 )
        {
            nearestPoint( coordinate, brick[i], givenBall );
            //If the ball enter the brick, come back.
            if( coordinate[0] == givenBall->centerX && coordinate[1] == givenBall->centerY )
            {
                givenBall->x -= givenBall->xVel;
                givenBall->y -= givenBall->yVel;
            }
            if( distance( givenBall->centerX, givenBall->centerY, coordinate[0], coordinate[1] ) <= givenBall->r )
            {
                if( brick[i].brick != 1 )
                {
                    brick[i].life -= 1;
                    //Increase the score.
                    scorePlus( brick[i] );
                    //If ball get the explode bonus, it will explode.
                    if( givenBall->explode == 1 )
                    {
                        ballExplode();
                    }
                }
                if( brick[i].life <= 0 )
                {
                    randomBonus( brick[i] );
                }
                //When the ball collide the brick, change the speed.
                if(coordinate[0] == brick[i].x && coordinate[1] == givenBall->centerY)
                {
                    if( givenBall->xVel < 0 )
                    {
                        givenBall->yVel = -givenBall->yVel;
                    }
                    else
                    {
                        givenBall->x -= givenBall->xVel;
                        givenBall->xVel = -givenBall->xVel;
                    }

                }
                else if(coordinate[0] == brick[i].x + brick[i].clip.w && coordinate[1] == givenBall->centerY)
                {
                    if( givenBall->xVel > 0 )
                    {
                        givenBall->yVel = -givenBall->yVel;
                    }
                    else
                    {
                        givenBall->x -= givenBall->xVel;
                        givenBall->xVel = -givenBall->xVel;
                    }
                }

                else if( coordinate[0] == brick[i].x && coordinate[1] == brick[i].y && givenBall->yVel < 0 )
                {
                    givenBall->xVel = -givenBall->xVel;
                }
                else if( coordinate[0] == brick[i].x + brick[i].clip.w && coordinate[1] == brick[i].y && givenBall->yVel <0 )
                {
                    givenBall->xVel = -givenBall->xVel;
                }
                else if( coordinate[0] == brick[i].x && coordinate[1] == brick[i].y + brick[i].clip.h && givenBall->yVel > 0 )
                {
                    givenBall->xVel = -givenBall->xVel;
                }
                else if( coordinate[0] == brick[i].x + brick[i].clip.w && coordinate[1] == brick[i].y + brick[i].clip.h && givenBall->yVel > 0 )
                {
                    givenBall->xVel = -givenBall->xVel;
                }
                else
                {
                    givenBall->y -= givenBall->yVel;
                    givenBall->yVel = -givenBall->yVel;
                }
                ballCenter();
                //If the collision happen, return 1.
                return 1;
            }
        }
    }
    return 0;
}

//Increase the score
void scorePlus( Brick givenBrick )
{
    if( givenBrick.life == 0)
    {
        if( givenBrick.brick == 2 )
        {
            text.score += 300;
        }
        else if( givenBrick.brick == 3 )
        {
            text.score += 100;
        }
    }
}

//Check whether the player win one level.
int winLevel()
{
    int i = 0;
    while( brick[i].life == 0 || brick[i].brick == 1 )
    {
        i++;
    }
    if( i == 100 )
    {
        return 1;
    }
    else
    {
        return 0;
    }

}

//Caculate the distance between the ball and a brick.
double distance( int x1, int y1, int x2, int y2 )
{

    return sqrt( pow( x2 - x1, 2 ) + pow( y2 - y1, 2 ) );
}


//To start the timer an begain to get the initial time.
void timeStart()
{
    timer.started = 1;
    //Save the initial time.
    timer.startTime = SDL_GetTicks();
}

//To get a time difference.
int getTime()
{
    if( timer.started == 1 )
    {
        if( timer.paused == 1 )
        {
            return timer.pausedTime;
        }
        else
        {
            return SDL_GetTicks() - timer.startTime;
        }
    }
    return 0;
}

//Make the game stop when the player win or lose and make the music stop.
void stopGame( int type )
{
    int quit = 1;
    ball.ballSpeed = 0;
    ball.xVel = 0;
    ball.yVel = 0;
    myPaddle.paddleSpeed = 0;
    myPaddle.vel = 0;
    bonus.bonusMode = -1;
    Mix_PlayChannel( -1, endSound, 0 );
    Mix_FreeMusic( music );
    while( quit )
    {

        applySurface( 0, 0, backGround, screen ,NULL);
        showBricks();
        applyPaddle();
        applySurface( ball.x, ball.y, ball.ballPic, screen ,&ball.clip[0]);
        applyBonus();
        showFont();
        //Check whether the player win or not.
        if( type == 1 )
        {
            applySurface(SCREEN_LEFT+60,100,endGame,screen,NULL);
        }
        else if( type == 2 )
        {
            applySurface(SCREEN_LEFT+60,100,winGame,screen,NULL);
            applySurface( button.x,button.y,button.button,screen,&button.clip[1]);
        }
        if( SDL_Flip( screen ) == -1 )
        {
            return ;
        }
        if( pause()==1 )
        {
            quit = 0;
        }
    }
}

//If the ball lose one life, it will recover something.
void lifeMinus()
{
    paddleInit();
    cloneBall[0].life = 0;
    cloneBall[1].life = 0;
    cloneBall[2].life = 0;
    ballInit();
    timer.paused = 1;
}

//Pause the game until press F5 or quit.
int pause()
{
    int stop=1;
    while(stop)
    {
        while(SDL_PollEvent(&event))
        {
            if(event.type==SDL_KEYDOWN)
            {
                switch(event.key.keysym.sym)
                {
                case SDLK_UP:
                    stop = 0;
                    break;
                case SDLK_F5:
                    text.restart = 1;
                    stop = 0;
                    return 1;
                    break;
                default :
                    break;
                }
            }
            else if(event.type==SDL_QUIT)
            {
                exit(0);
            }
            moveJudge();
        }
        SDL_Delay(100);
    }
    return 0;
}

//Initialize something for the next level.
void nextLevel()
{
    paddleInit();
    cloneBall[0].life = 0;
    cloneBall[1].life = 0;
    cloneBall[2].life = 0;
    ballInit();
    bricksInit();
    timer.paused = 1;
    bonus.bonusMode = -1;
    text.level ++;
    text.score = 0;
}
//Enter the next level and change something.
void enterNext()
{
    nextLevel();
    if( text.level == 2 )
    {
        brickLevel("levels/level2.txt");
    }
    else if( text.level == 3 )
    {
        brickLevel("levels/level3.txt");
    }

    blitPicture();
}

//For the last level's short cut.
int mouseEvent()
{
    //The mouse coordinate
    int x = 0, y = 0, control = 0;
    if( event.type == SDL_MOUSEBUTTONDOWN )
    {
        //If the left mouse button was pressed
        if( event.button.button == SDL_BUTTON_LEFT )
        {
            //Get the mouse offsets
            x = event.button.x;
            y = event.button.y;

            //If the mouse is over the button
            if( ( x > button.x ) && ( x < button.x + button.w ) && ( y > button.y ) && ( y < button.y + button.h ) )
            {
                button.num = 0;
            }
        }
    }
    //If a mouse button was released
    if( event.type == SDL_MOUSEBUTTONUP )
    {
        //If the left mouse button was released
        if( event.button.button == SDL_BUTTON_LEFT )
        {
            //Get the mouse offsets
            x = event.button.x;
            y = event.button.y;

            //If the mouse is over the button
            if( ( x > button.x ) && ( x < button.x + button.w ) && ( y > button.y ) && ( y < button.y + button.h ) )
            {
                button.num = 1;
                control = 1;
            }
        }
    }
    return control;
}


