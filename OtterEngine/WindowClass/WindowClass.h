#pragma once

#include "OtterEngine/MinimalWindow.h"

class WindowClass {
public:
	WindowClass(LPCWSTR wndClassName, LPCTSTR wndTitle, UINT32 width, UINT32 height);
	~WindowClass();

	// prevent copying
	WindowClass(const WindowClass&) = delete;
	WindowClass& operator= (const WindowClass&) = delete;

private:
	static LRESULT WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

private:
	HWND m_hWnd;
	HINSTANCE m_hInstance;
	LPCWSTR m_wndClassName;
	LPCWSTR m_wndTitle;
	UINT32 m_width;
	UINT32 m_height;
};