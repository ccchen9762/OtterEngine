#include "ImguiManager.h"

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

#include "OtterEngine/Game/Game.h"

#include "OtterEngine/Common/constants.h"

ImguiManager::ImguiManager() {
    // ============================== Setup ImGui context ==============================
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
}

ImguiManager::~ImguiManager() {
    // Cleanup
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}

void ImguiManager::Update(Game& game) {
    
    if (kShowWindow) {
        ImGui::Begin("System Panel", &kShowWindow, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

        ImGuiIO& io = ImGui::GetIO(); (void)io;
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
        ImGui::Spacing();
        ImGui::Checkbox("Show Debug items", &game.showDebug);

        ImGui::End();
    }
}
