#include "Game.h"

#include <string>

#include "OtterEngine/Imgui/imgui.h"
#include "OtterEngine/Imgui/imgui_impl_win32.h"
#include "OtterEngine/Imgui/imgui_impl_dx11.h"

#include "OtterEngine/Entity/Cube.h"
#include "OtterEngine/Entity/Triangle.h"

#include "OtterEngine/Common/Randomizer.h"
#include "OtterEngine/Common/constants.h"

Game::Game() :
    m_imguiManager(ImguiManager()),
    m_mainWindow(Window(kDefWndTitle, kRenderWidth, kRenderHeight)), 
    m_alive(true), 
    m_timer(Timer()),
    m_camera(Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, -1.0f), Vector3(0.0f, 1.0f, 0.0f), 
        DirectX::XM_PIDIV4, kRenderRatio, 0.1f, 100.0f, 0.02f, 0.1f) {

    Randomizer::Init();

    for (int i = 0; i < 20; i++) {
        m_renderList.push_back(std::make_unique<Cube>(
            *(m_mainWindow.m_pGraphics),
            Vector3(Randomizer::GetFloat(static_cast<float>(kPI)), Randomizer::GetFloat(static_cast<float>(kPI)), 0.0f),
            Vector3(Randomizer::GetFloat(5.0f, -5.0f), Randomizer::GetFloat(5.0f, -5.0f), -20.0f),
            Vector3(0.0f, 0.0f, 0.0f),
            Vector3(1.0f, 1.0f, 1.0f),
            m_camera.GetViewProjectionMatrix(),
            Randomizer::GetFloat(0.08f, 0.02f)
        ));
    }
}

int Game::Start() {
    // message loop
    MSG msg = {};
    while (m_alive) {

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
                        const std::wstring title = L"X: " + std::to_wstring(pos.x) + L", Y: " + std::to_wstring(pos.y);
                        m_mainWindow.setTitle(title);

                        static Vector3Int prevPos = pos;    // only initialized once
                        if (m_mainWindow.m_mouse.IsMButtonPressed()) {
                            m_camera.TranslateCamera(pos, prevPos);
                        }
                        if (m_mainWindow.m_mouse.IsRButtonPressed()) {
                            m_camera.RotateCamera(pos, prevPos);
                        }
                        prevPos = pos;
                        break;
                    }
                    }
                }
            }

            if (msg.message == WM_QUIT) {
                m_alive = false;
            }
        }

        Update();
    }

    // WM_QUIT.wParam is parameter of PostQuitMessage
	return static_cast<int>(msg.wParam);
}

void Game::Update() {

    m_timer.Update();

    m_mainWindow.m_pGraphics->ClearBuffer(0.1f, 0.1f, 0.1f);
    
    for (int i = 0; i < m_renderList.size(); i++) {
        m_renderList[i]->Update();
        m_renderList[i]->Render(*(m_mainWindow.m_pGraphics));
    }

    m_imguiManager.Update();

    m_mainWindow.m_pGraphics->PostUpdate();
}


