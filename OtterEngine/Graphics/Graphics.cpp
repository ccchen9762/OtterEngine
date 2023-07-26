#include "Graphics.h"

#pragma comment(lib, "d3d11.lib")		// definitions
#pragma comment(lib, "d3dcompiler.lib")

Graphics::Graphics(HWND hWnd) {

	// desc stands for descriptor
	DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
	swapChainDesc.BufferDesc.Width = 0;		// 0 means let it figure out
	swapChainDesc.BufferDesc.Height = 0;	//
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 0;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapChainDesc.SampleDesc.Count = 1;		// for anti aliasing
	swapChainDesc.SampleDesc.Quality = 0;	//
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = 1;	// 1 frame buffer, 1 back buffer
	swapChainDesc.OutputWindow = hWnd;
	swapChainDesc.Windowed = true;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Flags = 0;

	DX::ThrowIfFailed(D3D11CreateDeviceAndSwapChain(
		nullptr,					// IDXGIAdapter*, pass nullptr to use default
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,					// software rasterizer, pass nullptr for driver type other than software
		D3D11_CREATE_DEVICE_DEBUG,
		nullptr,					// D3D feature level, pass nullptr for default array
		0,							// number of elements in feature level
		D3D11_SDK_VERSION,
		&swapChainDesc,
		&m_pSwapChain,
		&m_pDevice,
		nullptr,					// feature level, nullptr if no need to specify 
		&m_pDeviceContext
	));

	Microsoft::WRL::ComPtr<ID3D11Texture2D> pBackBuffer;
	DX::ThrowIfFailed(m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer));
	if (pBackBuffer)
		DX::ThrowIfFailed(m_pDevice->CreateRenderTargetView(pBackBuffer.Get(), nullptr, &m_pRenderTarget));
}

void Graphics::ClearBuffer(float red, float green, float blue) {
	const float color[4] = {red, green, blue, 1.0f};

	m_pDeviceContext->ClearRenderTargetView(m_pRenderTarget.Get(), color);
}

void Graphics::Update() {

	PostUpdate();
}

void Graphics::PostUpdate() {
	m_pSwapChain->Present(1u, 0u); // swapchain present need to be call at the end of frame
}
