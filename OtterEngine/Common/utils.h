#pragma once

inline bool MouseInsideWindow(int x, int y, unsigned int width, unsigned int height) {
	return x >= 0 && x < static_cast<int>(width) && 
		   y >= 0 && y < static_cast<int>(height);
}