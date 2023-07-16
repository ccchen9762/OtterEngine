#include "WindowClass.h"

WindowClass::WindowClass(HINSTANCE hInstance, LPCWSTR wndClassName, LPCTSTR wndTitle,
		UINT32 width, UINT32 height) :
	m_hInstance(hInstance),
	m_WndClassName(wndClassName) {

	// register window class
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_OWNDC; // allow to render multiple windows independently
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = m_hInstance;
	wcex.hIcon = nullptr;
	wcex.hCursor = nullptr;
	wcex.hbrBackground = nullptr;
	wcex.lpszMenuName = nullptr;
	wcex.lpszClassName = m_WndClassName;
	wcex.hIconSm = nullptr;
	RegisterClassEx(&wcex);

	RECT clientSpace = { 0 };
	clientSpace.left = 100;
	clientSpace.right = width + 100;
	clientSpace.top = 100;
	clientSpace.bottom = height + 100;
	AdjustWindowRect(&clientSpace, WS_OVERLAPPEDWINDOW, false);


	m_hWnd = CreateWindowEx(0, m_WndClassName, L"Otter Engine", WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		clientSpace.right - clientSpace.left, clientSpace.bottom - clientSpace.top,
		nullptr, nullptr, m_hInstance, this);


	SetCapture(m_hWnd);

	ShowWindow(m_hWnd, SW_SHOW);
	UpdateWindow(m_hWnd);
}

LRESULT WINAPI WindowClass::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	return DefWindowProc(hWnd, msg, wParam, lParam);
}
