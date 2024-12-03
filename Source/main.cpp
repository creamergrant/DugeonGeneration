#include <GameDev2D.h>
#include "Game.h"


//Entry point to the application
int main(int argc, char** argv)
{
#if CHECK_FOR_MEMORY_LEAKS
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

    using namespace GameDev2D;

    //Create the Window's PlatformLayer and initialize it and set the Root Scene
    Application application;

    //Create the Game object
    Game* game = new Game();

    //Register the callbacks
    application.UpdateSlot.connect<&Game::OnUpdate>(game);
    application.RenderSlot.connect<&Game::OnRender>(game);
    application.GetInputManager().GetKeyboard().KeyEventSlot.connect<&Game::OnKeyEvent>(game);
    application.GetInputManager().GetMouse().MouseButtonEventSlot.connect<&Game::OnMouseButtonEvent>(game);
    application.GetInputManager().GetMouse().MouseMovedEventSlot.connect<&Game::OnMouseMovedEvent>(game);

    //Trigger the application's game loop, it will not return until the application is ready to close
    application.Run();

    //Delete the Game object
    delete game;

    return 0;
}

