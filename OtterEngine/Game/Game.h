#pragma once

#include <memory>

#include "OtterEngine/Imgui/ImguiManager.h" 
#include "OtterEngine/WindowClass/Window.h"
#include "OtterEngine/Common/Timer.h"
#include "OtterEngine/Light/PointLight.h"
#include "OtterEngine/Entity/Entity.h"
#include "OtterEngine/Camera/Camera.h"

#include "OtterEngine/Entity/Model.h"

class Game {
	
	friend class ImguiManager;

public:
	Game();
	~Game() = default;
	
	// prevent copying
	Game(const Game&) = delete;
	Game& operator= (const Game&) = delete;

	int Start();
	bool ProcessMessages(int& wParam);

private:
	void HandleInput(double deltaTime);
	void Update(double deltaTime);

private:
	ImguiManager m_imguiManager; // order matters!! Imgui must be initialized before Window
	Window m_mainWindow;
	bool m_alive;
	Timer m_timer;
	std::vector<std::unique_ptr<PointLight>> m_lightList;
	std::vector<std::unique_ptr<Entity>> m_debugList;
	std::vector<std::unique_ptr<Entity>> m_renderList;
	Camera m_camera;
	bool showDebug;

	Model m_model;
};