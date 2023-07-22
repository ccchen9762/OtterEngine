#pragma once

#include "OtterEngine/WindowClass/Window.h"
#include "OtterEngine/Common/Constants.h"

class Game {
public:
	Game();
	~Game() = default;
	
	// prevent copying
	Game(const Game&) = delete;
	Game& operator= (const Game&) = delete;

	int Start();

private:
	Window m_window;
};