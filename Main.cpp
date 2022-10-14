#include <iostream>
#include "SDLGame.h"
#include "CatGameStateMachine.h"

int main(int argc, char* argv[])

{
    SDLGame game;
    CatGameStateMachine gameInstance(&game);
    game.InitSDL(&gameInstance);
    game.RunGameLoop();
    game.DeinitSDL();

    std::cout << "Have " << argc << " arguments:" << std::endl;
    for (int i = 0; i < argc; ++i) {
        std::cout << argv[i] << std::endl;
    }
    system("pause");
    return 0;

}