#pragma once

#include "OtterEngine/Common/MinimalWindows.h"
#include "OtterEngine/Input/Keyboard.h"
#include "OtterEngine/Input/Mouse.h"

class Window {
public:
	Window(LPCTSTR wndTitle, UINT32 width, UINT32 height);
	~Window();

	// prevent copying
	Window(const Window&) = delete;
	Window& operator= (const Window&) = delete;

	void setTitle(const std::wstring& title) { SetWindowText(m_hWnd, title.c_str()); }
	void CaptureWindow() { SetCapture(m_hWnd); }

private:
	static LRESULT DefWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	static LRESULT WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	LRESULT MessageHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

public:
	Keyboard m_keyboard;
	Mouse m_mouse;

private:
	HWND m_hWnd;
	HINSTANCE m_hInstance;
	LPCWSTR m_wndClassName;
	LPCWSTR m_wndTitle;
	UINT32 m_width;
	UINT32 m_height;
};