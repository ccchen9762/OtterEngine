#include "Game.h"

#include <string>

#include "OtterEngine/Common/Randomizer.h"
#include "OtterEngine/Common/utils.h"

Game::Game() :
    m_mainWindow(Window(kDefWndTitle, kRenderWidth, kRenderHeight)), m_mainWindowAlive(true), m_timer(Timer()) {

    Randomizer::Init();
}

int Game::Start() {
    // message loop
    MSG msg = {};
    while (m_mainWindowAlive) {

        /*  nullptr in GetMessage() / PeekMessage() means to retrieve messages from any window belongs to current thread
            GetMessage() waits until a message comes in, return 0 means WM_QUIT, return -1 means error
            PeekMessage() always tries to get message, return bool */
        
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);

            if (m_mainWindow.m_keyboard.IsKeyPressed(VK_TAB) || m_mainWindow.m_keyboard.IsKeyPressed(VK_MENU)) {
                unsigned int result = MessageBox(nullptr, L"HIHIHI", L"TMEPY", MB_OK);
                if (result == IDOK)
                    m_mainWindow.CaptureWindow();
            }

            while (!m_mainWindow.m_mouse.MouseEventBufferEmpty()) {
                const Mouse::MouseEvent mouseEvent = m_mainWindow.m_mouse.ReadFirstEvent();

                static int test = 0;
                if (mouseEvent.getEventType() != Mouse::MouseEvent::Type::Empty) {
                    switch (mouseEvent.getEventType()) {
                    case Mouse::MouseEvent::Type::Leave:
                        m_mainWindow.setTitle(L"outside");
                        break;
                    case Mouse::MouseEvent::Type::WheelDown: {
                        --test;
                        const std::wstring title = std::to_wstring(test);
                        m_mainWindow.setTitle(title);
                        break;
                    }
                    case Mouse::MouseEvent::Type::WheelUp: {
                        ++test;
                        const std::wstring title = std::to_wstring(test);
                        m_mainWindow.setTitle(title);
                        break;
                    }
                    case Mouse::MouseEvent::Type::Move: {
                        Vector3Int pos = mouseEvent.getPosition();
                        const std::wstring title = L"X: " + std::to_wstring(pos.m_x) + L", Y: " + std::to_wstring(pos.m_y);
                        m_mainWindow.setTitle(title);
                        break;
                    }
                    }
                }
            }

            if (msg.message == WM_QUIT) {
                m_mainWindowAlive = false;
            }
        }

        Update();
    }

    // WM_QUIT.wParam is parameter of PostQuitMessage
	return msg.wParam;
}

void Game::Update() {
    m_timer.Update();

    m_mainWindow.m_pGraphics->ClearBuffer(0.1f, 0.1f, 0.1f);
    m_mainWindow.m_pGraphics->CreateRenderResource();
    // m_mainWindow.m_pGraphics->DrawTriangle(kPI/4);
    // m_mainWindow.m_pGraphics->DrawCube(m_timer.GetElapsedTimeSecond(), m_timer.GetElapsedTimeSecond() / 2, 0.0f);
    m_mainWindow.m_pGraphics->DrawCube(Randomizer::GetFloat(kPI), Randomizer::GetFloat(kPI), 0.0f);
    m_mainWindow.m_pGraphics->PostUpdate();
}


