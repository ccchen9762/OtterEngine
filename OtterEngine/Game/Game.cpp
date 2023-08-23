#include "Game.h"

#include <string>

#include "OtterEngine/Imgui/imgui.h"
#include "OtterEngine/Imgui/imgui_impl_win32.h"
#include "OtterEngine/Imgui/imgui_impl_dx11.h"

#include "OtterEngine/Entity/DebugEntity/DebugLine.h"
#include "OtterEngine/Entity/ShadingEntity/Cube.h"
#include "OtterEngine/Entity/ShadingEntity/Sphere.h"
#include "OtterEngine/Entity/ShadingEntity/Plane.h"
#include "OtterEngine/Entity/Model/Character.h"

#include "OtterEngine/Common/Randomizer.h"
#include "OtterEngine/Common/constants.h"

Game::Game() :
    m_imguiManager(ImguiManager()),
    m_mainWindow(Window(kDefWndTitle, kRenderWidth, kRenderHeight)), 
    m_alive(true), 
    m_timer(Timer()),
    m_camera(*(m_mainWindow.m_pGraphics), Vector3(10.0f, 8.0f, 15.0f), Vector3(-0.5f, -0.33f, -1.0f), Vector3(0.0f, 1.0f, 0.0f)),
    showDebug(true),
    m_model(*(m_mainWindow.m_pGraphics),
        Vector3(8.0f, 0.0f, -4.0f),
        Vector3(0.0f, 0.0f, 0.0f),
        Vector3(1.0f, 1.0f, 1.0f),
        m_camera,
        false,
        "Assets/Model/test.glb"){ //"Assets/Model/nano_hierarchy.gltf"

    Randomizer::Init();

    // render debug axis
    m_debugList.push_back(std::make_unique<DebugLine>(
        *(m_mainWindow.m_pGraphics),
        Vector3(0.0f, 0.0f, 0.0f),
        Vector3(0.0f, 0.0f, 0.0f),
        Vector3(2.0f, 2.0f, 2.0f),
        m_camera,
        Color4{1.0f, 0.0f, 0.0f, 1.0f},
        true
        ));
    m_debugList.push_back(std::make_unique<DebugLine>(
        *(m_mainWindow.m_pGraphics),
        Vector3(0.0f, 0.0f, 0.0f),
        Vector3(0.0f, 0.0f, static_cast<float>(kPI/2)),
        Vector3(2.0f, 2.0f, 2.0f),
        m_camera,
        Color4{ 0.0f, 1.0f, 0.0f, 1.0f },
        true
    ));
    m_debugList.push_back(std::make_unique<DebugLine>(
        *(m_mainWindow.m_pGraphics),
        Vector3(0.0f, 0.0f, 0.0f),
        Vector3(0.0f, static_cast<float>(-kPI/2), 0.0f),
        Vector3(2.0f, 2.0f, 2.0f),
        m_camera,
        Color4{ 0.0f, 0.0f, 1.0f, 1.0f },
        true
    ));

    m_lightList.push_back(std::make_unique<PointLight>(*(m_mainWindow.m_pGraphics), 
        DirectX::XMFLOAT4{ 0.0f, 8.0f, 4.0f, 1.0f }, 
        Color4{ 1.0f, 0.7f, 0.7f, 1.0f }, 
        1.0f, m_camera));

    /*for (int i = 0; i < 30; i++) {
        m_renderList.push_back(std::make_unique<Cube>(
            *(m_mainWindow.m_pGraphics),
            Vector3(Randomizer::GetFloat(-5.0f, 5.0f), Randomizer::GetFloat(0.0f, 10.0f), Randomizer::GetFloat(-9.0f, 1.0f)),
            Vector3(Randomizer::GetFloat(static_cast<float>(kPI)), Randomizer::GetFloat(static_cast<float>(kPI)), 0.0f),
            Vector3(1.0f, 1.0f, 1.0f),
            m_camera,
            false
        ));
    }

    for (int i = 0; i < 10; i++) {
        m_renderList.push_back(std::make_unique<Sphere>(
            *(m_mainWindow.m_pGraphics),
            Vector3(Randomizer::GetFloat(-5.0f, 5.0f), Randomizer::GetFloat(0.0f, 10.0f), Randomizer::GetFloat(-9.0f, 1.0f)),
            Vector3(Randomizer::GetFloat(static_cast<float>(kPI)), Randomizer::GetFloat(static_cast<float>(kPI)), 0.0f),
            Vector3(1.0f, 1.0f, 1.0f),
            m_camera,
            false
        ));
    }*/

    m_renderList.push_back(std::make_unique<Character>(
        *(m_mainWindow.m_pGraphics),
        Vector3(-8.0f, 0.0f, -4.0f),
        Vector3(0.0f, 0.0f, 0.0f),
        Vector3(1.0f, 1.0f, 1.0f),
        m_camera,
        "",
        false
    ));

    // floor
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            m_renderList.push_back(std::make_unique<Plane>(
                *(m_mainWindow.m_pGraphics),
                Vector3(-14.0f + i*4, 0.0f, -14.0f + j*4),
                Vector3(0.0f, 0.0f, 0.0f),
                Vector3(4.0f, 1.0f, 4.0f),
                m_camera,
                L"Assets\\Texture\\wood.jpg",
                true
            ));
        }
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

            HandleInput();

            if (msg.message == WM_QUIT) {
                m_alive = false;
            }
        }
        else {  // Magic!!! "else" prevents slow down when large amount of inputs come in
            Update();
        }
    }

    // wParam of WM_QUIT is parameter of PostQuitMessage
	return static_cast<int>(msg.wParam);
}

void Game::HandleInput() {
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
                m_camera.TranslateCameraZ(-1);
                break;
            }
            case Mouse::MouseEvent::Type::WheelUp: {
                ++test;
                const std::wstring title = std::to_wstring(test);
                m_mainWindow.setTitle(title);
                m_camera.TranslateCameraZ(1);
                break;
            }
            case Mouse::MouseEvent::Type::Move: {
                Vector3Int pos = mouseEvent.getPosition();
                const std::wstring title = L"X: " + std::to_wstring(pos.x) + L", Y: " + std::to_wstring(pos.y);
                m_mainWindow.setTitle(title);

                static Vector3Int prevPos = pos; // static will only be initialized once
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
}

void Game::Update() {

    m_timer.Update();

    m_mainWindow.m_pGraphics->ClearBuffer(0.1f, 0.1f, 0.1f);
    
    m_camera.Update(*(m_mainWindow.m_pGraphics));

    m_lightList[0]->Update(*(m_mainWindow.m_pGraphics));
    m_lightList[0]->Render(*(m_mainWindow.m_pGraphics));

    if(showDebug) {
        for (int i = 0; i < m_debugList.size(); i++) {
            m_debugList[i]->Update();
            m_debugList[i]->Render(*(m_mainWindow.m_pGraphics));
        }
    }

    for (int i = 0; i < m_renderList.size(); i++) {
        m_renderList[i]->Update();
        m_renderList[i]->Render(*(m_mainWindow.m_pGraphics));
    }

    m_model.Render(*(m_mainWindow.m_pGraphics));

    // Start the Dear ImGui frame
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    m_imguiManager.Update(*this);
    m_lightList[0]->ShowControlWindow();
    m_model.ShowControlWindow();

    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

    m_mainWindow.m_pGraphics->PostUpdate();
}


