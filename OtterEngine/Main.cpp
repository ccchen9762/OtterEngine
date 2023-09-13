#include "OtterEngine/Game/Game.h"
#include "OtterEngine/Common/Randomizer.h"

// basic game loop: process input -> update game -> render

int WINAPI WinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance, // always NULL
    _In_ PSTR lpCmdLine,
    _In_ int nCmdShow) {

    Randomizer::Init();

    Game game;

    return game.Start();
}