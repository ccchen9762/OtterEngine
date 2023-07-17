#include "Game.h"

Game::Game() :
    m_wndClass(WindowClass(kDefWndTitle, kRenderWidth, kRenderHeight)) {
}

int Game::Start() {

    // message loop
    MSG msg;
    bool running = true;
    
    // nullptr in GetMessage means to retrieve messages from any window belongs to current thread
    // GetMessage() return 0 means WM_QUIT, return -1 means error, so check if message is greater than 0
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);

        if (msg.message == WM_QUIT) {
            running = false;
        }
    }

	return 0;
}


