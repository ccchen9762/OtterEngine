#pragma once

#include "OtterEngine/WindowClass/Window.h"
#include "OtterEngine/Imgui/ImguiManager.h"
#include "OtterEngine/Common/Timer.h"
#include "OtterEngine/Common/constants.h"

class Game {
public:
	Game();
	~Game() = default;
	
	// prevent copying
	Game(const Game&) = delete;
	Game& operator= (const Game&) = delete;

	int Start();

private:
	void Update();

private:
	ImguiManager m_imguiManager; // order matters!! Imgui must be initialized before Window
	Window m_mainWindow;
	bool m_mainWindowAlive;
	Timer m_timer;
};