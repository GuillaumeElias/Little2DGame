/*This source code copyrighted by Lazy Foo' Productions (2004-2015)
and may not be redistributed without written permission.*/

//Using SDL and standard IO
#include <SDL.h>
#include <stdio.h>
#include <Game.h>
#include <Utils/LTimer.h>
#include <Player.h>
#include "src/Constants.h"


//Screen dimension constants

int main( int argc, char* args[] )
{
    Game* game = new Game();
    game->run();

	return 0;
}

