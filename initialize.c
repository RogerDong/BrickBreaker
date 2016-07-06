#include "cFile.h"



/*Use this file to initialize everything and prepare for the game.*/



//initialize SDL and make a screen for the game.
int init()
{
    //Check whether it can start nomally.
    if( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
    {
        return 0;
    }
    //Create a screen with its width and height.
    screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE );
    if( screen == NULL )
    {
        return 0;
    }
    //Initialize the SDL_mixer
    if( Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 2, 4096 ) == -1 )
    {
        return 0;
    }
    //Initialize the SDL_ttf
    if( TTF_Init() == -1 )
    {
        return 0;
    }
    //Load all music and picture
    if( loadFiles() == 0 )
    {
        return 0;
    }
    //Initialize many other things
    paddleInit();
    myPaddle.vel = 0;
    ballInit();
    ball.life = 3;
    cloneBall[0].life = 0;
    cloneBall[1].life = 0;
    cloneBall[2].life = 0;
    buttonInit();
    bonusInit();
    textInit();
    bricksInit();
    timer.paused = 1;
    brickLevel("levels/level1.txt");
    //Set a caption.
    SDL_WM_SetCaption( "Brick Breaker", NULL );
    //Initialize time function
    srand(time(NULL));
    return 1;
}


//Initialize the paddle with its coordinates and speed.
void paddleInit()
{
    myPaddle.paddleWidth = 80;
    myPaddle.paddleHeight = 20;
    myPaddle.stick = 0;
    myPaddle.length = 0;
    myPaddle.x = (SCREEN_WIDTH-SCREEN_LEFT-SCREEN_RIGHT)/2+SCREEN_LEFT-myPaddle.paddleWidth/2;
    myPaddle.y = SCREEN_HEIGHT-myPaddle.paddleHeight*2;
    myPaddle.paddleSpeed = 10;
}

//Initialize the ball with its coordinates, speed, width, height and so on.
void ballInit()
{
    int i = 0, x = 0;
    ball.ballSpeed = 8;
    ball.ballWidth = 20;
    ball.ballHeight = 20;
    ball.xVel = ball.ballSpeed*cos(PI/4);
    ball.yVel = -ball.ballSpeed*sin(PI/4);
    ball.x = myPaddle.x + myPaddle.paddleWidth/2;
    ball.y = SCREEN_HEIGHT-myPaddle.paddleHeight*2-ball.ballHeight;
    //Calculate the ball center
    ballCenter();
    ball.r = ball.ballHeight/2;
    ball.explode = 0;
    for( i=0; i<4; i++)
    {
        ball.clip[i].h = 20;
        ball.clip[i].w = 20;
        ball.clip[i].y = 0;
        ball.clip[i].x = x;
        x += 20;
    }

    //Initialize the clone balls
    for( i=0; i<3; i++)
    {
        cloneBall[i].ballSpeed = 8;
        cloneBall[i].ballWidth = 20;
        cloneBall[i].ballHeight = 20;
        cloneBall[i].xVel = ball.ballSpeed*cos(PI/4);
        cloneBall[i].yVel = -ball.ballSpeed*sin(PI/4);
        cloneBall[i].x = SCREEN_WIDTH/2;
        cloneBall[i].y = SCREEN_HEIGHT-myPaddle.paddleHeight*2-ball.ballHeight;
        cloneBall[i].r = ball.ballHeight/2;
        cloneBall[i].explode = 0;

    }


}

//Initialize the bonus structure and prepare for the game.
void bonusInit()
{
    int i = 0,x = 0;
    bonus.bonusWidth = 10;
    bonus.bonusHeight = 25;
    bonus.bonusMode = -1;
    bonus.yVel = 4;
    for( i=0; i<8; i++)
    {
        bonus.clip[i].x = x;
        bonus.clip[i].y = 0;
        bonus.clip[i].h = 25;
        bonus.clip[i].w = 10;
        x += 10;
    }
}

//Initialize the structure of text.
//Level and score variable will show in the screen
void textInit()
{

    text.level = 1;
    text.score = 0;
    text.restart = 0;
}

//Initialize the button for passing the last level, including its coordinate and so on
void buttonInit()
{
    button.x = 120;
    button.y = 220;
    button.h = 50;
    button.w = 150;
    button.clip[0].h = 50;
    button.clip[0].w = 150;
    button.clip[0].x = 0;
    button.clip[0].y = 0;
    button.clip[1].h = 50;
    button.clip[1].w = 150;
    button.clip[1].x = 0;
    button.clip[1].y = 50;
    button.num = -1;
}

//Caculate the center of the ball and the clone ball.
void ballCenter()
{
    int i = 0;
    ball.centerX = ball.x+ball.ballHeight/2;
    ball.centerY = ball.y+ball.ballHeight/2;
    for( i=0; i<3; i++)
    {
        if( cloneBall[i].life > 0 )
        {
            cloneBall[i].centerX = cloneBall[i].x + ball.ballHeight/2;
            cloneBall[i].centerY = cloneBall[i].y + ball.ballHeight/2;
        }
    }
}



//Read the listing bricks in the file
void brickLevel( const char *fileName )
{
    int i = 0;
    FILE *pToLevel;
    pToLevel = fopen( fileName,"r" );

    for( i=0; i<100; i++)
    {
        //Everytime read the number of brick's type in the file number
        fscanf( pToLevel, "%1d", &brick[i].brick );
        if( brick[i].brick == 1 )
        {
            brick[i].life = 1;
            brick[i].clip.w = 52;
            brick[i].clip.h = 25;
            brick[i].clip.x = 0;
            brick[i].clip.y = 0;
        }
        else if( brick[i].brick == 2 )
        {
            brick[i].life = 3;
            brick[i].clip.w = 52;
            brick[i].clip.h = 25;
            brick[i].clip.x = 0;
            brick[i].clip.y = 25;
        }
        else if( brick[i].brick == 3 )
        {
            brick[i].life = 1;
            brick[i].clip.w = 52;
            brick[i].clip.h = 25;
            brick[i].clip.x = 0;
            brick[i].clip.y = 50;
        }

    }
    fclose( pToLevel );
}



//Initialize every bricks' location in the screen and give it life, transparency and so on.
void bricksInit()
{
    int i = 0,j = 0;
    int x = SCREEN_LEFT + 1,y = 70;
    for( i=0; i<10; i++)
    {
        for(j=0; j<10; j++)
        {
            brick[i*10+j].x = x;
            brick[i*10+j].y = y;
            brick[i*10+j].life = 0;
            brick[i*10+j].alpha = 255;
            x += 52;
        }
        x = SCREEN_LEFT + 1;
        y += 26;
    }
}



//Show bricks in the screen.
void showBricks()
{
    int i = 0;
    for(i=0; i<100; i++)
    {
        if(brick[i].alpha > 0)
        {
            if( brick[i].life > 0 )
            {
                SDL_SetAlpha( brickPic, SDL_SRCALPHA, brick[i].alpha );

            }
            else if( brick[i].life == 0 )
            {
                //Disappear the bricks in several frames
                SDL_SetAlpha( brickPic, SDL_SRCALPHA, brick[i].alpha );
                brick[i].alpha -= 17;
            }
            applySurface( brick[i].x, brick[i].y, brickPic, screen, &brick[i].clip );
        }

    }
}


//Print the string with one font to the screen
void print(int x,int y,const char *str,int color,int fontSize)
{
    text.font = TTF_OpenFont("fonts/calibrib.ttf",fontSize);
    SDL_FreeSurface(message);
    if( color == 1 )
    {
        message = TTF_RenderText_Solid(text.font,str,fontColor);
    }
    applySurface(x,y,message,screen,NULL);
    TTF_CloseFont( text.font );
}

//Transfer the integer to string and put it into the screen
void showFont()
{
    char sScore[10],sLevel[10],sLife[10];
    //Transfer the integer to the string
    sprintf(sLevel,"%d",text.level);	//把整数转化为字符串
    sprintf(sScore,"%d",text.score);
    sprintf(sLife,"%d",ball.life);
    print(SCREEN_LEFT+260,10,sLevel,1,30);
    print(SCREEN_WIDTH-70,10,sScore,1,30);
    print(SCREEN_LEFT+160,10,"Level",1,30);
    print(SCREEN_LEFT+350,10,"Score",1,30);

    print(SCREEN_LEFT+80,10,sLife,1,30);
    print(SCREEN_LEFT+10,10,"Life",1,30);
}

//To load a image and optimize the image.
SDL_Surface *loadImage( const char *fileName, int type )
{
    SDL_Surface* image = NULL;
    SDL_Surface* newImage = NULL;
    image = IMG_Load( fileName );
    if( image != NULL )
    {
        newImage = SDL_DisplayFormat( image );
        SDL_FreeSurface( image );
        //If the area is white and the type is 1,the area will become transparent.
        if( newImage != NULL )
        {
            if( type == 1 )
            {
                SDL_SetColorKey( newImage, SDL_SRCCOLORKEY, SDL_MapRGB( newImage->format, 0xFF, 0xFF, 0xFF ) );
            }
        }
    }

    return newImage;
}

//To load all the pictures and vioce and other files.
int loadFiles()
{
    button.button = loadImage("images/button.png",0);
    backGround = loadImage( "images/background.png",0 );
    brickPic = loadImage("images/brick.png",0);
    endGame = loadImage("images/gameOver.png",1);
    winGame = loadImage("images/win.png",1);
    if( backGround == NULL || brick == NULL || endGame == NULL || button.button == NULL || winGame == NULL )
    {
        return 0;
    }

    music = Mix_LoadMUS("sounds/music.wav");
    collision = Mix_LoadWAV("sounds/collision.wav");
    endSound = Mix_LoadWAV("sounds/gameover.wav");
    explode = Mix_LoadWAV("sounds/explode.wav");
    if( music == NULL || collision == NULL || endSound == NULL || explode == NULL )
    {
        return 0;
    }

    ball.ballPic = loadImage("images/ball.png",1);
    myPaddle.paddle = loadImage("images/paddle.png",0);
    myPaddle.increase = loadImage("images/paddleIncrease.png",0);
    if( ball.ballPic == NULL || myPaddle.paddle == NULL || myPaddle.increase == NULL )
    {
        return 0;
    }
    bonus.bonus = loadImage( "images/bonus.png" ,0);
    if( bonus.bonus == NULL )
    {
        return 0;
    }
    cloneBall[0].ballPic = loadImage("images/cloneBall.png",1);
    cloneBall[1].ballPic = loadImage("images/cloneBall.png",1);
    cloneBall[2].ballPic = loadImage("images/cloneBall.png",1);
    if( cloneBall[0].ballPic == NULL || cloneBall[1].ballPic == NULL || cloneBall[2].ballPic == NULL )
    {
        return 0;
    }
    return 1;
}

//To apply the surface to the screen.
void applySurface( int x, int y, SDL_Surface* source, SDL_Surface* dest, SDL_Rect* clip  )
{
    SDL_Rect rect;
    rect.x = x;
    rect.y = y;
    SDL_BlitSurface( source, clip, dest, &rect );
}

//To apply the picture of paddle to the screen whether it become wider or not
void applyPaddle()
{
    SDL_Rect clip[2];
    clip[0].x = 0;
    clip[0].y = 0;
    clip[0].h = 20;
    clip[0].w = 40;

    clip[1].x = 40;
    clip[1].y = 0;
    clip[1].h = 20;
    clip[1].w = 40;
    //To apply the part of increase if it become wider
    if( myPaddle.length == 0 )
    {
        applySurface( myPaddle.x, myPaddle.y, myPaddle.paddle, screen ,NULL);
    }
    else if( myPaddle.length == 1 )
    {
        applySurface( myPaddle.x, myPaddle.y, myPaddle.paddle, screen , &clip[0]);
        applySurface( myPaddle.x+myPaddle.paddle->w/2, myPaddle.y, myPaddle.increase, screen , NULL);
        applySurface( myPaddle.x+myPaddle.paddle->w/2+myPaddle.increase->w, myPaddle.y, myPaddle.increase, screen , NULL);
        applySurface( myPaddle.x+myPaddle.paddle->w/2+myPaddle.increase->w*2, myPaddle.y, myPaddle.paddle, screen , &clip[1]);
    }
    else if( myPaddle.length == 2 )
    {
        applySurface( myPaddle.x, myPaddle.y, myPaddle.paddle, screen , &clip[0]);
        applySurface( myPaddle.x+myPaddle.paddle->w/2, myPaddle.y, myPaddle.increase, screen , NULL);
        applySurface( myPaddle.x+myPaddle.paddle->w/2+myPaddle.increase->w, myPaddle.y, myPaddle.increase, screen , NULL);
        applySurface( myPaddle.x+myPaddle.paddle->w/2+myPaddle.increase->w*2, myPaddle.y, myPaddle.increase, screen , NULL);
        applySurface( myPaddle.x+myPaddle.paddle->w/2+myPaddle.increase->w*3, myPaddle.y, myPaddle.increase, screen , NULL);
        applySurface( myPaddle.x+myPaddle.paddle->w/2+myPaddle.increase->w*4, myPaddle.y, myPaddle.paddle, screen , &clip[1]);
    }
    else if( myPaddle.length == 3 )
    {
        applySurface( myPaddle.x, myPaddle.y, myPaddle.paddle, screen , &clip[0]);
        applySurface( myPaddle.x+myPaddle.paddle->w/2, myPaddle.y, myPaddle.increase, screen , NULL);
        applySurface( myPaddle.x+myPaddle.paddle->w/2+myPaddle.increase->w, myPaddle.y, myPaddle.increase, screen , NULL);
        applySurface( myPaddle.x+myPaddle.paddle->w/2+myPaddle.increase->w*2, myPaddle.y, myPaddle.increase, screen , NULL);
        applySurface( myPaddle.x+myPaddle.paddle->w/2+myPaddle.increase->w*3, myPaddle.y, myPaddle.increase, screen , NULL);
        applySurface( myPaddle.x+myPaddle.paddle->w/2+myPaddle.increase->w*4, myPaddle.y, myPaddle.increase, screen , NULL);
        applySurface( myPaddle.x+myPaddle.paddle->w/2+myPaddle.increase->w*5, myPaddle.y, myPaddle.increase, screen , NULL);
        applySurface( myPaddle.x+myPaddle.paddle->w/2+myPaddle.increase->w*6, myPaddle.y, myPaddle.paddle, screen , &clip[1]);
    }

}

//To make the bonus fall radomly
void randomBonus( Brick givenBrick )
{
    const int MAX = 7, MIN = 0;
    int random = 0;
    random = (rand() % 2);
    if( text.score % 300 == 0 )  //text.score % 500 == 0
    {
        if( random == 1 )
        {
            random = (rand() % (MAX - MIN + 1)) + MIN;
            bonus.x = givenBrick.x + BRICK_WIDTH/2 - bonus.bonusWidth/2;
            bonus.y = givenBrick.y + BRICK_HEIGHT;
            bonus.bonusMode = random;
        }
    }


}

//Make the bonus' picture fall in the screen
void applyBonus()
{

    if( bonus.bonusMode <= 7 && bonus.bonusMode >= 0 )
    {
        applySurface( bonus.x, bonus.y, bonus.bonus, screen, &bonus.clip[bonus.bonusMode] );
        bonus.y += bonus.yVel;
    }

}

//Make the ball moving animation
void rollBall()
{
    if( ball.animate % 4 == 1 )
    {
        applySurface(ball.x,ball.y,ball.ballPic,screen,&ball.clip[0]);
    }
    else if( ball.animate % 4 ==2 )
    {
        applySurface(ball.x,ball.y,ball.ballPic,screen,&ball.clip[1]);
    }
    else if( ball.animate % 4 == 3 )
    {
        applySurface(ball.x,ball.y,ball.ballPic,screen,&ball.clip[2]);
    }
    else if( ball.animate % 4 == 0 )
    {
        applySurface(ball.x,ball.y,ball.ballPic,screen,&ball.clip[3]);
    }
    ball.animate ++ ;
}




//This function will apply all sufaces to the screen.
void blitPicture()
{
    //SDL_SetAlpha( backGround, SDL_SRCALPHA, 170 );
    applySurface( 0, 0, backGround, screen ,NULL);
    if( button.num >= 0 )
    {
        applySurface( button.x,button.y,button.button,screen,&button.clip[button.num]);
    }
    showBricks();
    applyPaddle();
    if( myPaddle.stick == 2 )
    {
        applySurface(ball.x,ball.y,ball.ballPic,screen,&ball.clip[0]);
    }
    else
    {
        rollBall();
    }
    if( cloneBall[0].life > 0 )
    {
        applySurface( cloneBall[0].x, cloneBall[0].y, cloneBall[0].ballPic, screen ,NULL);
    }
    if( cloneBall[1].life > 0 )
    {
        applySurface( cloneBall[1].x, cloneBall[1].y, cloneBall[1].ballPic, screen ,NULL);
    }
    if( cloneBall[2].life > 0 )
    {
        applySurface( cloneBall[2].x, cloneBall[2].y, cloneBall[2].ballPic, screen ,NULL);
    }
    applyBonus();
    showFont();
    if( SDL_Flip( screen ) == -1 )
    {
        return ;
    }

}

//To free sufaces and other things,then quit the SDl.
void freeGame()
{
    SDL_FreeSurface( backGround );
    SDL_FreeSurface( button.button );
    SDL_FreeSurface( brickPic );
    SDL_FreeSurface( endGame );
    SDL_FreeSurface( winGame );
    SDL_FreeSurface( ball.ballPic );
    SDL_FreeSurface( myPaddle.paddle );
    SDL_FreeSurface( myPaddle.increase );
    SDL_FreeSurface( bonus.bonus );
    SDL_FreeSurface( cloneBall[0].ballPic );
    SDL_FreeSurface( cloneBall[1].ballPic );
    SDL_FreeSurface( cloneBall[2].ballPic );

    SDL_FreeSurface( message );
    TTF_CloseFont( text.font );
    TTF_Quit();

    Mix_FreeMusic( music );
    Mix_FreeChunk( collision );
    Mix_FreeChunk( explode );

    Mix_FreeChunk( endSound );
    SDL_Quit();

}

//This funtion is prepared to save game to txt file
void saveGame()
{
    int i = 0;
    FILE *pToSave;
    pToSave = fopen( "save.txt","w" );
    fprintf( pToSave, "%d\t%d\n", text.level , text.score);
    fprintf( pToSave, "%d\t%d\t%d\n", myPaddle.paddleSpeed, myPaddle.x, myPaddle.y);
    fprintf( pToSave, "%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n", ball.centerX, ball.centerY, ball.ballSpeed, ball.r, ball.x, ball.xVel, ball.y, ball.yVel);
    for( i=0; i<60; i++)
    {
        fprintf( pToSave, "%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n", brick[i].brick, brick[i].clip.h, brick[i].clip.w, brick[i].clip.x, brick[i].clip.y , brick[i].x, brick[i].y, brick[i].life);
    }
    fclose( pToSave );
}
//This funtion is prepared to load game to txt file
void loadGame()
{
    int i = 0;
    int h[60] = {0}, w[60] = {0}, x[60] = {0}, y[60] = {0};
    int coordinateX[60] = {0}, coordinateY[60] = {0};
    FILE *pToSave;
    pToSave = fopen( "save.txt","r" );
    fscanf( pToSave, "%d\t%d\n", &text.level , &text.score);
    fscanf( pToSave, "%d\t%d\t%d\n", &myPaddle.paddleSpeed, &myPaddle.x, &myPaddle.y);
    fscanf( pToSave, "%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n", &ball.centerX, &ball.centerY, &ball.ballSpeed, &ball.r, &ball.x, &ball.xVel, &ball.y, &ball.yVel);
    for( i=0; i<60; i++)
    {
        fscanf( pToSave, "%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n", &brick[i].brick, &h[i], &w[i], &x[i], &y[i] , &coordinateX[i], &coordinateY[i], &brick[i].life);
    }
    for( i=0; i<60; i++)
    {
        brick[i].clip.h = h[i];
        brick[i].clip.w = w[i];
        brick[i].clip.x = x[i];
        brick[i].clip.y = y[i];
        brick[i].x = coordinateX[i];
        brick[i].y = coordinateY[i];
    }

    fclose( pToSave );


}







