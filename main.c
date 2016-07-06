#include "cFile.h"



int main(int argc,char *args[])
{
    //Initialize the game and play the music
    startGame();
    //The main part of the game
    baseGame();
    //Free the all surface and music from memory
    freeGame();
    return 0;
}
