#include "Core/Game.h"

int main(int argc, char* argv[])
{
    Game newGame;
    newGame.Initialize();
    newGame.RunLoop();

    return 0;
}