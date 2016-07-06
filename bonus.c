#include "cFile.h"


//Clone the ballif get the clone bonus.
void clone()
{
    int i=0;
    myPaddle.stick = 0;
    for( i=0; i<3; i++)
    {
        //If the clone ball is less than 3 balls, clone one ball.
        if( cloneBall[i].life == 0 )
        {
            cloneBall[i].centerX = ball.centerX;
            cloneBall[i].centerY = ball.centerY;
            cloneBall[i].life = 1;
            cloneBall[i].x = ball.x;
            cloneBall[i].y = ball.y;
            cloneBall[i].xVel = -ball.xVel;
            cloneBall[i].yVel = ball.yVel;
            cloneBall[i].ballSpeed = ball.ballSpeed;
            break;
        }
    }
}





//Make the sticky ball move with the paddle.
void stickball()
{

    ball.xVel = 0;
    ball.yVel = 0;
    if( myPaddle.stick == 2 )
    {
        ball.x +=  myPaddle.stickVel;
    }
}

//If paddle get the bonus, it will return touch number 1.If touch number is 2, it falls under the bottom.
int bonusCollision()
{
    int touch = 0;
    if( bonus.x + bonus.bonusWidth <= myPaddle.x )
    {
        touch = 0;
    }
    else if( bonus.x >= myPaddle.x + myPaddle.paddleWidth )
    {
        touch = 0;
    }
    else if( bonus.y + bonus.bonusHeight <= myPaddle.y )
    {
        touch = 0;
    }
    else if( bonus.y >= myPaddle.y + myPaddle.paddleWidth )
    {
        touch = 0;
    }
    else
    {
        touch = 1;
    }
    if( bonus.y >= SCREEN_HEIGHT )
    {
        touch = 2;
    }
    return touch;
}

//If the paddle get the bonus, it will get the influence.
void bonusEffect()
{
    int control = 0;
    int i = 0;
    control = bonusCollision();
    //bonus.bonusMode = 3;
    if( control == 1 || control == 2 )
    {
        if( control == 1 )
        {
            if( bonus.bonusMode == 0 )
            {
                if( myPaddle.length < 3 )
                {
                    myPaddle.length++;
                    myPaddle.paddleWidth += 20;
                }
            }
            else if( bonus.bonusMode == 1 )
            {
                myPaddle.stick = 1;
                cloneBall[0].life = 0;
                cloneBall[1].life = 0;
                cloneBall[2].life = 0;
            }
            else if( bonus.bonusMode == 2 )
            {
                clone();
            }
            else if( bonus.bonusMode == 3 )
            {
                ball.explode = 1;
            }
            else if( bonus.bonusMode == 4 )
            {

                if( ball.ballSpeed > 4 )
                {
                    ball.ballSpeed -= 2;
                    ball.xVel *= (float)(ball.ballSpeed*1.0 / (ball.ballSpeed + 2));
                    ball.yVel *= (float)(ball.ballSpeed*1.0 / (ball.ballSpeed + 2));
                    for( i=0; i<3; i++ )
                    {
                        if( cloneBall[i].life > 0 )
                        {
                            cloneBall[i].ballSpeed -= 2;
                            cloneBall[i].xVel *= (float)(cloneBall[i].ballSpeed *1.0 / (cloneBall[i].ballSpeed + 2));
                            cloneBall[i].yVel *= (float)(cloneBall[i].ballSpeed *1.0 / (cloneBall[i].ballSpeed + 2));
                        }
                    }
                }
            }
            else if( bonus.bonusMode == 5 )
            {
                if( myPaddle.length > 0 )
                {
                    myPaddle.length--;
                    myPaddle.paddleWidth -= 20;
                }
            }
            else if( bonus.bonusMode == 6 )
            {
                ball.life -= 1;
                if( ball.life > 0)
                {
                    lifeMinus();
                }
            }
            else if( bonus.bonusMode == 7 )
            {
                if( ball.ballSpeed < 10 )
                {
                    ball.ballSpeed += 2;
                    ball.xVel *= (float)ball.ballSpeed / (ball.ballSpeed - 2);
                    ball.yVel *= (float)ball.ballSpeed / (ball.ballSpeed - 2);
                    for( i=0; i<3; i++ )
                    {
                        if( cloneBall[i].life > 0 )
                        {
                            cloneBall[i].ballSpeed += 2;
                            cloneBall[i].xVel *= (float)cloneBall[i].ballSpeed / (cloneBall[i].ballSpeed - 2);
                            cloneBall[i].yVel *= (float)cloneBall[i].ballSpeed / (cloneBall[i].ballSpeed - 2);
                        }
                    }
                }
            }
        }
        bonus.bonusMode = -1;
        bonus.x = 0;

    }

}

//The explosion bonus will explode the bricks within the range of 4*r.
void ballExplode()
{
    int i = 0, point[2] = {0};
    for( i=0; i<100 ; i++)
    {
        if( brick[i].life > 0 )
        {
            nearestPoint( point, brick[i], &ball );
            if( distance( ball.centerX, ball.centerY, point[0], point[1] ) < 4*ball.r && brick[i].brick != 1 )
            {
                brick[i].life = 0;
                //Make the score increase
                if( brick[i].brick == 2 )
                {
                    text.score += 300;
                }
                else if( brick[i].brick == 3 )
                {
                    text.score += 100;
                }
            }
        }

    }

}

