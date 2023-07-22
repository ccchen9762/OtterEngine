#include "Window.h"

#include "OtterEngine/Common/Constants.h"

Window::Window(LPCTSTR wndTitle, UINT32 width, UINT32 height) :
	m_wndClassName(kDefWndClassName), m_wndTitle(wndTitle),
	m_width(width), m_height(height), m_hInstance() {

	// register window class
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_OWNDC; // allow to render multiple windows independently
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = GetModuleHandle(nullptr);
	wcex.hIcon = nullptr;
	wcex.hCursor = nullptr;
	wcex.hbrBackground = nullptr;
	wcex.lpszMenuName = nullptr;
	wcex.lpszClassName = kDefWndClassName;
	wcex.hIconSm = nullptr;
	RegisterClassEx(&wcex);

	// adjust client space
	RECT clientSpace;
	clientSpace.left = 100;
	clientSpace.right = width + 100;
	clientSpace.top = 100;
	clientSpace.bottom = height + 100;
	AdjustWindowRect(&clientSpace, WS_OVERLAPPEDWINDOW, false);

	// create window instance
	// last parameter is additional, use it to point to this instance
	m_hWnd = CreateWindowEx(0, m_wndClassName, m_wndTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		clientSpace.right - clientSpace.left, clientSpace.bottom - clientSpace.top,
		nullptr, nullptr, GetModuleHandle(nullptr), this);

	ShowWindow(m_hWnd, SW_SHOW);
}

Window::~Window() {
	UnregisterClass(m_wndClassName, m_hInstance);
	DestroyWindow(m_hWnd);
}

LRESULT Window::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	Window* pWindow = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	return pWindow->MessageHandler(hWnd, msg, wParam, lParam);
}

LRESULT Window::MessageHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
	case WM_KILLFOCUS: {
		//m_keyboard.ClearKeyState();
		break;
	}
	case WM_KEYDOWN:
		if (wParam == 'F')
			SetWindowText(hWnd, L"123");
		break;
	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;	// to prevent DefWindowProc destroy this instance, let destructor does it
		break;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}
