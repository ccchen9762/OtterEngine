#pragma once

#include <Windows.h>

class WindowClass {
public:
	WindowClass(HINSTANCE hInstance, LPCWSTR wndClassName, LPCTSTR wndTitle, UINT32 width, UINT32 height);
	~WindowClass() = default;

private:
	static LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

private:
	HWND m_hWnd;
	HINSTANCE m_hInstance;
	LPCWSTR m_WndClassName;
};