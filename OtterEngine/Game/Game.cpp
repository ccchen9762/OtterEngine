#include "Game.h"

#include <string>

#include "OtterEngine/Imgui/imgui.h"
#include "OtterEngine/Imgui/imgui_impl_win32.h"
#include "OtterEngine/Imgui/imgui_impl_dx11.h"

#include "OtterEngine/Entity/DebugEntity/DebugLine.h"
#include "OtterEngine/Entity/ShadingEntity/Plane.h"
#include "OtterEngine/Entity/ShadingEntity/Cube.h"
#include "OtterEngine/Entity/ShadingEntity/Sphere.h"
#include "OtterEngine/Entity/Model/Character.h"
#include "OtterEngine/Entity/Model/Sponza.h"

#include "OtterEngine/Common/Randomizer.h"
#include "OtterEngine/Common/constants.h"

Game::Game() :
    m_imguiManager(ImguiManager()),
    m_mainWindow(Window(kDefWndTitle, kRenderWidth, kRenderHeight)), 
    m_alive(true), 
    m_timer(Timer()),
    m_camera(*(m_mainWindow.m_pGraphics), Vector3(10.0f, 15.0f, 0.0f), Vector3(-0.5f, 0.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f)),
    m_directionalLights(*(m_mainWindow.m_pGraphics)),
    m_pointLights(*(m_mainWindow.m_pGraphics)),
    showDebug(true) {

    Randomizer::Init();

    m_directionalLights.AddLight(
        *(m_mainWindow.m_pGraphics),
        DirectX::XMFLOAT4{ -1.0f, -1.0f,-1.0f, 0.0f },
        Color4{ 1.0f, 1.0f, 1.0f, 1.0f });

    m_pointLights.AddLight(this,
        *(m_mainWindow.m_pGraphics),
        DirectX::XMFLOAT4{ 0.0f, 8.0f, 4.0f, 1.0f },
        Color4{ 1.0f, 0.7f, 0.7f, 1.0f });

    m_pointLights.Render(*(m_mainWindow.m_pGraphics));

    // render debug axis
    m_debugList.push_back(std::make_unique<DebugLine>(
        this,
        *(m_mainWindow.m_pGraphics),
        Vector3(0.0f, 0.0f, 0.0f),
        Vector3(0.0f, 0.0f, 0.0f),
        Vector3(2.0f, 2.0f, 2.0f),
        Color4{1.0f, 0.0f, 0.0f, 1.0f},
        true
        ));

    m_debugList.push_back(std::make_unique<DebugLine>(
        this,
        *(m_mainWindow.m_pGraphics),
        Vector3(0.0f, 0.0f, 0.0f),
        Vector3(0.0f, 0.0f, static_cast<float>(kPI/2)),
        Vector3(2.0f, 2.0f, 2.0f),
        Color4{ 0.0f, 1.0f, 0.0f, 1.0f },
        true
    ));
    m_debugList.push_back(std::make_unique<DebugLine>(
        this,
        *(m_mainWindow.m_pGraphics),
        Vector3(0.0f, 0.0f, 0.0f),
        Vector3(0.0f, static_cast<float>(-kPI/2), 0.0f),
        Vector3(2.0f, 2.0f, 2.0f),
        Color4{ 0.0f, 0.0f, 1.0f, 1.0f },
        true
    ));

    /*for (int i = 0; i < 30; i++) {
        m_renderList.push_back(std::make_unique<Cube>(
            this,
            *(m_mainWindow.m_pGraphics),
            Vector3(Randomizer::GetFloat(-5.0f, 5.0f), Randomizer::GetFloat(0.0f, 10.0f), Randomizer::GetFloat(-9.0f, 1.0f)),
            Vector3(Randomizer::GetFloat(static_cast<float>(kPI)), Randomizer::GetFloat(static_cast<float>(kPI)), 0.0f),
            Vector3(1.0f, 1.0f, 1.0f),
            false
        ));
    }

    for (int i = 0; i < 10; i++) {
        m_renderList.push_back(std::make_unique<Sphere>(
            this,
            *(m_mainWindow.m_pGraphics),
            Vector3(Randomizer::GetFloat(-5.0f, 5.0f), Randomizer::GetFloat(0.0f, 10.0f), Randomizer::GetFloat(-9.0f, 1.0f)),
            Vector3(Randomizer::GetFloat(static_cast<float>(kPI)), Randomizer::GetFloat(static_cast<float>(kPI)), 0.0f),
            Vector3(1.0f, 1.0f, 1.0f),
            false
        ));
    }*/

    // floor
    /*for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            m_renderList.push_back(std::make_unique<Plane>(
                this,
                *(m_mainWindow.m_pGraphics),
                Vector3(-14.0f + i*4, 0.0f, -14.0f + j*4),
                Vector3(0.0f, 0.0f, 0.0f),
                Vector3(4.0f, 1.0f, 4.0f),
                L"Assets\\Texture\\wood.jpg",
                L"",
                true
            ));
        }
    }

    m_renderList.push_back(std::make_unique<Plane>(
        this,
        *(m_mainWindow.m_pGraphics),
        Vector3(0.0f, 5.0f, -2.0f),
        Vector3(kPI/2, 0.0f, 0.0f),
        Vector3(10.0f, 1.0f, 10.0f),
        L"Assets\\Texture\\brickwall\\brickwall.jpg",
        L"Assets\\Texture\\brickwall\\brickwall_normal.jpg",
        true
    ));*/

    /*m_modelList.push_back(std::make_unique<Character>(
        this, *(m_mainWindow.m_pGraphics),
        Vector3(8.0f, 0.0f, -4.0f),
        Vector3(0.0f, 0.0f, 0.0f),
        Vector3(1.0f, 1.0f, 1.0f),
        false,
        "Assets/Model/nanosuit/nanosuit.obj")
    );*/

    m_modelList.push_back(std::make_unique<Sponza>(
        this, *(m_mainWindow.m_pGraphics),
        Vector3(0.0f, 0.0f, 0.0f),
        Vector3(0.0f, 0.0f, 0.0f),
        Vector3(0.1f, 0.1f, 0.1f),
        false,
        "Assets/Model/sponza/sponza.obj")
    );
}

int Game::Start() {
    // message loop
    int wParam = 0;
    while (m_alive) {
        // move message handling loop outside the main loop
        // so it will handle pending messages but not wait for new messages
        if (!ProcessMessages(wParam))
            break;

        double deltaTime = m_timer.Update();
        HandleInput(deltaTime);
        Update(deltaTime);
    }

    return wParam;
    // wParam of WM_QUIT is parameter of PostQuitMessage
	//return static_cast<int>(msg.wParam);
}

bool Game::ProcessMessages(int& wParam) {

    /*  nullptr in GetMessage() / PeekMessage() means to retrieve messages from any window belongs to current thread
            GetMessage() waits until a message comes in, return 0 means WM_QUIT, return -1 means error
            PeekMessage() always tries to get message, return bool */

    MSG msg = {};
    while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);

        if (msg.message == WM_QUIT) {
            wParam = msg.wParam;
            m_alive = false;
            break;
        }
    }

    return m_alive;
}


void Game::HandleInput(double deltaTime) {
    if (m_mainWindow.m_keyboard.IsKeyPressed(VK_TAB) || m_mainWindow.m_keyboard.IsKeyPressed(VK_MENU)) {
        unsigned int result = MessageBox(nullptr, L"HIHIHI", L"TMEPY", MB_OK);
        if (result == IDOK)
            m_mainWindow.CaptureWindow();
    }

    // W
    if (m_mainWindow.m_keyboard.IsKeyPressed(0x57)) {
        m_camera.TranslateCameraZ(deltaTime * 3000.0f);
    }
    // S
    if (m_mainWindow.m_keyboard.IsKeyPressed(0x53)) {
        m_camera.TranslateCameraZ(-deltaTime * 3000.0f);
    }
    // A
    if (m_mainWindow.m_keyboard.IsKeyPressed(0x41)) {
        m_camera.TranslateCamera(-deltaTime * 2000.0f, 0.0f);
    }
    // D
    if (m_mainWindow.m_keyboard.IsKeyPressed(0x44)) {
        m_camera.TranslateCamera(deltaTime * 2000.0f, 0.0f);
    }
    // space
    if (m_mainWindow.m_keyboard.IsKeyPressed(VK_SPACE)) {
        // minus goes up
        m_camera.TranslateCamera(0.0f, -deltaTime * 2000.0f);
    }
    // L Shift
    if (m_mainWindow.m_keyboard.IsKeyPressed(VK_SHIFT)) {
        m_camera.TranslateCamera(0.0f, deltaTime * 2000.0f);
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
                m_camera.TranslateCameraZ(deltaTime * -200000.0f);
                break;
            }
            case Mouse::MouseEvent::Type::WheelUp: {
                ++test;
                const std::wstring title = std::to_wstring(test);
                m_mainWindow.setTitle(title);
                m_camera.TranslateCameraZ(deltaTime * 200000.0f);
                break;
            }
            case Mouse::MouseEvent::Type::Move: {
                const Vector3Int& pos = mouseEvent.getPosition();
                const std::wstring title = L"X: " + std::to_wstring(pos.x) + L", Y: " + std::to_wstring(pos.y);
                m_mainWindow.setTitle(title);

                static Vector3Int prevPos = pos; // static will only be initialized once
                float translateX = pos.x - prevPos.x;
                float translateY = pos.y - prevPos.y;
                if (m_mainWindow.m_mouse.IsMButtonPressed()) {
                    m_camera.TranslateCamera(translateX, translateY);
                }
                if (m_mainWindow.m_mouse.IsRButtonPressed()) {
                    m_camera.RotateCamera(translateX, translateY);
                }
                prevPos = pos;
                break;
            }
            }
        }
    }
}

void Game::Update(double deltaTime) {

    m_mainWindow.m_pGraphics->ClearBuffer(0.15f, 0.15f, 0.15f);
    
    m_camera.Update(*(m_mainWindow.m_pGraphics));

    m_directionalLights.Update(*(m_mainWindow.m_pGraphics));

    m_pointLights.Update(*(m_mainWindow.m_pGraphics));
    m_pointLights.Render(*(m_mainWindow.m_pGraphics));

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

    for (int i = 0; i < m_modelList.size(); i++) {
        m_modelList[i]->Render(*(m_mainWindow.m_pGraphics));
    }

    // Start the Dear ImGui frame
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    m_imguiManager.Update(*this);
    m_directionalLights.ShowControlWindow();
    m_pointLights.ShowControlWindow();

    for (int i = 0; i < m_modelList.size(); i++) {
        m_modelList[i]->ShowControlWindow();
    }

    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

    m_mainWindow.m_pGraphics->PostUpdate();
}


