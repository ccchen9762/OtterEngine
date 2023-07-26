#pragma once

#include "OtterEngine/Common/pch.h"

#include "OtterEngine/Input/Keyboard.h"
#include "OtterEngine/Input/Mouse.h"
#include "OtterEngine/Graphics/Graphics.h"

class Window {
public:
	Window(LPCTSTR wndTitle, const unsigned int width, const unsigned int height);
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
	Graphics* m_pGraphics; // use pointer to wait for HWND for construction
	Keyboard m_keyboard;
	Mouse m_mouse;

private:
	HWND m_hWnd;
	HINSTANCE m_hInstance;
	LPCTSTR m_wndTitle;
	unsigned int m_width;
	unsigned int m_height;
};