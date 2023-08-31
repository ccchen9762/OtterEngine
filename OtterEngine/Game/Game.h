#pragma once

#include <memory>

#include "OtterEngine/Imgui/ImguiManager.h" 
#include "OtterEngine/WindowClass/Window.h"
#include "OtterEngine/Camera/Camera.h"
#include "OtterEngine/Light/PointLight.h"
#include "OtterEngine/Light/DirectionalLight.h"
#include "OtterEngine/Entity/Entity.h"
#include "OtterEngine/Entity/Model/Model.h"
#include "OtterEngine/Common/Timer.h"

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

	const Camera& GetCurrentCamera() const { return m_camera; }

private:
	void HandleInput(double deltaTime);
	void Update(double deltaTime);

private:
	ImguiManager m_imguiManager; // order matters!! Imgui must be initialized before Window
	Window m_mainWindow;
	bool m_alive;
	Timer m_timer;

	// camera -> lights -> objects
	Camera m_camera;
	DirectionalLight m_directionalLights;
	PointLight m_pointLights;
	std::vector<std::unique_ptr<Entity>> m_debugList;
	std::vector<std::unique_ptr<Entity>> m_renderList;
	std::vector<std::unique_ptr<Model>> m_modelList;
	bool showDebug;
};