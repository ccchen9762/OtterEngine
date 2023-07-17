#pragma once

#include "OtterEngine/Common/MinimalWindows.h"

class WindowClass {
public:
	WindowClass(LPCTSTR wndTitle, UINT32 width, UINT32 height);
	~WindowClass();

	// prevent copying
	WindowClass(const WindowClass&) = delete;
	WindowClass& operator= (const WindowClass&) = delete;

private:
	static LRESULT WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	LRESULT MessageHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

private:
	static WindowClass* s_pWnd;

	HWND m_hWnd;
	HINSTANCE m_hInstance;
	LPCWSTR m_wndClassName;
	LPCWSTR m_wndTitle;
	UINT32 m_width;
	UINT32 m_height;
};