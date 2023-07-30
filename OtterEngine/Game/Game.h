#pragma once

#include "OtterEngine/WindowClass/Window.h"
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
	Window m_mainWindow;
	bool m_mainWindowAlive;
	Timer m_timer;
};