#include "Window.h"

#include "OtterEngine/Common/constants.h"
#include "OtterEngine/Common/utils.h"

Window::Window(LPCTSTR wndTitle, UINT32 width, UINT32 height) :
	m_wndTitle(wndTitle), m_width(width), m_height(height), 
    m_hInstance(GetModuleHandle(nullptr)), m_pGraphics(nullptr) {

	// register window class
    WNDCLASSEX wcex = {};
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_OWNDC; // allow to render multiple windows independently
	wcex.lpfnWndProc = DefWndProc;
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
    RECT clientSpace = {};
	clientSpace.left = 100;
	clientSpace.right = width + 100;
	clientSpace.top = 100;
	clientSpace.bottom = height + 100;
	AdjustWindowRect(&clientSpace, WS_OVERLAPPEDWINDOW, false);

	// create window instance
	// last parameter is additional, use it to point to this instance
	m_hWnd = CreateWindowEx(0, kDefWndClassName, m_wndTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		clientSpace.right - clientSpace.left, clientSpace.bottom - clientSpace.top,
		nullptr, nullptr, GetModuleHandle(nullptr), this);

	ShowWindow(m_hWnd, SW_SHOW);

    m_pGraphics = new Graphics(m_hWnd);
}

Window::~Window() {

    delete m_pGraphics;

	UnregisterClass(kDefWndClassName, m_hInstance);

	DestroyWindow(m_hWnd);
}

LRESULT Window::DefWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    // the very first message accociated with window
    if (msg == WM_NCCREATE) {
        // extract pointer of window class
        CREATESTRUCT* pCreateStruct = reinterpret_cast<CREATESTRUCT*>(lParam);
        Window* pWindow = reinterpret_cast<Window*>(pCreateStruct->lpCreateParams);
        // store this pointer to additional user data 
        SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWindow));
        // switch message handle function to window instance memeber function
        SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&WndProc));
        // let it handle the rest
        return pWindow->MessageHandler(hWnd, msg, wParam, lParam);
    }
    // return default until WM_NCCREATE
    return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT Window::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	// use pointer passed in CreateWindowEx to identify which window instance
	Window* pWindow = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
    return pWindow->MessageHandler(hWnd, msg, wParam, lParam);
}

LRESULT Window::MessageHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
    case WM_KILLFOCUS: {
        m_keyboard.ClearKeyState();
        break;
    }
	/* ========== Mouse handling begin ========== */
    case WM_MOUSEMOVE: {
        const POINTS mousePos = MAKEPOINTS(lParam);
        if (MouseInsideWindow(mousePos.x, mousePos.y, m_width, m_height)) {
            m_mouse.OnMouseMove(Vector3Int(mousePos.x, mousePos.y));
            if (!m_mouse.IsInWindow()) {
                if(GetForegroundWindow() == hWnd)
                    SetCapture(hWnd);
                m_mouse.OnMouseEnter();
            }
        }
        else {
            if (wParam & (MK_LBUTTON | MK_MBUTTON | MK_RBUTTON)) {
                m_mouse.OnMouseMove(Vector3Int(mousePos.x, mousePos.y));
            }
            else {
                ReleaseCapture();
                m_mouse.OnMouseLeave();
            }
        }
        break;
    }
    case WM_LBUTTONDOWN: {
        SetForegroundWindow(hWnd);
        SetCapture(hWnd);
        const POINTS mousePos = MAKEPOINTS(lParam);
        m_mouse.OnLButtonDown(Vector3Int(mousePos.x, mousePos.y));
        break;
    }
    case WM_MBUTTONDOWN: {
        SetForegroundWindow(hWnd);
        SetCapture(hWnd);
        const POINTS mousePos = MAKEPOINTS(lParam);
        m_mouse.OnMButtonDown(Vector3Int(mousePos.x, mousePos.y));
        break;
    }
    case WM_RBUTTONDOWN: {
        SetForegroundWindow(hWnd);
        SetCapture(hWnd);
        const POINTS mousePos = MAKEPOINTS(lParam);
        m_mouse.OnRButtonDown(Vector3Int(mousePos.x, mousePos.y));
        break;
    }
    case WM_LBUTTONUP: {
        const POINTS mousePos = MAKEPOINTS(lParam);
        m_mouse.OnLButtonUp(Vector3Int(mousePos.x, mousePos.y));
        if (!MouseInsideWindow(mousePos.x, mousePos.y, m_width, m_height)) {
            ReleaseCapture();
            m_mouse.OnMouseLeave();
        }
        break;
    }
    case WM_MBUTTONUP: {
        const POINTS mousePos = MAKEPOINTS(lParam);
        m_mouse.OnMButtonUp(Vector3Int(mousePos.x, mousePos.y));
        if (!MouseInsideWindow(mousePos.x, mousePos.y, m_width, m_height)) {
            ReleaseCapture();
            m_mouse.OnMouseLeave();
        }
        break;
    }
    case WM_RBUTTONUP: {
        const POINTS mousePos = MAKEPOINTS(lParam);
        m_mouse.OnRButtonUp(Vector3Int(mousePos.x, mousePos.y));
        if (!MouseInsideWindow(mousePos.x, mousePos.y, m_width, m_height)) {
            ReleaseCapture();
            m_mouse.OnMouseLeave();
        }
        break;
    }
    case WM_MOUSEWHEEL: {
        const POINTS mousePos = MAKEPOINTS(lParam);
        const int delta = GET_WHEEL_DELTA_WPARAM(wParam);
        m_mouse.OnWheelScrolled(Vector3Int(mousePos.x, mousePos.y), delta);
        break;
    }
	/* =========== Mouse handling end =========== */
	/* ========== Keyboard handling begin ========== */
	case WM_KEYDOWN:
    case WM_SYSKEYDOWN: {
		m_keyboard.OnKeyDown(static_cast<unsigned char>(wParam));
		break;
    }
	case WM_KEYUP:
    case WM_SYSKEYUP: {
		m_keyboard.OnKeyUp(static_cast<unsigned char>(wParam));
		break;
    }
    case WM_CHAR: {
		m_keyboard.OnCharEntered(static_cast<unsigned char>(wParam));
		break;
    }
	/* ========== = Keyboard handling end =========== */
    case WM_CLOSE: {
		PostQuitMessage(777);
		return 0;	// to prevent DefWindowProc destroy this instance, let destructor does it
		break;
    }
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}
