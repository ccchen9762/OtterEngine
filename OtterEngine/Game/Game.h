#pragma once

#include <memory>

#include "OtterEngine/WindowClass/Window.h"
#include "OtterEngine/Imgui/ImguiManager.h"
#include "OtterEngine/Entity/Entity.h"
#include "OtterEngine/Common/Timer.h"

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
	bool m_alive;
	std::vector<std::unique_ptr<Entity>> m_renderList;
	Timer m_timer;
};