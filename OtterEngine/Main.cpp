#include "OtterEngine/WindowClass/WindowClass.h"

// basic game loop:
//  process input -> update game -> render

int WINAPI WinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance, // always NULL
    _In_ PSTR lpCmdLine,
    _In_ int nCmdShow) {

    WindowClass(hInstance , L"D3D11_WINDOW_CLASS", L"Otter Engine", 1080u, 720u);

    GetMessage(
        nullptr, // set to null will retrieve messages from any window belongs to current thread
    );
    while (true);
    return 0;
}