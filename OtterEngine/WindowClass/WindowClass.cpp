#include "WindowClass.h"

WindowClass::WindowClass(LPCWSTR wndClassName, LPCTSTR wndTitle, 
		UINT32 width, UINT32 height) :
	m_wndClassName(wndClassName), m_wndTitle(wndTitle),
	m_width(width), m_height(height), m_hInstance(GetModuleHandle(nullptr)) {

	// register window class
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_OWNDC; // allow to render multiple windows independently
	wcex.lpfnWndProc = WndProc;	// pointer to WndPro
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = m_hInstance;
	wcex.hIcon = nullptr;
	wcex.hCursor = nullptr;
	wcex.hbrBackground = nullptr;
	wcex.lpszMenuName = nullptr;
	wcex.lpszClassName = m_wndClassName;
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
	m_hWnd = CreateWindowEx(0, m_wndClassName, m_wndTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		clientSpace.right - clientSpace.left, clientSpace.bottom - clientSpace.top,
		nullptr, nullptr, GetModuleHandle(nullptr), this);

	ShowWindow(m_hWnd, SW_SHOW);
}

WindowClass::~WindowClass() {
	UnregisterClass(m_wndClassName, m_hInstance);
	DestroyWindow(m_hWnd);
}

LRESULT WindowClass::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
	case WM_KEYDOWN:
		if (wParam == 'F')
			SetWindowText(hWnd, L"FFFFFF");
		break;
    case WM_CLOSE:
        PostQuitMessage(0);
        return 0;
        break;
    }

	return DefWindowProc(hWnd, msg, wParam, lParam);
}
