#include "Graphics.h"

#include <DirectXMath.h>

#include "OtterEngine/Imgui/imgui_impl_dx11.h"

#include "Resource/VertexShader.h"
#include "Resource/PixelShader.h"
#include "Resource/InputLayout.h"
#include "Resource/VertexBuffer.h"
#include "Resource/IndexBuffer.h"
#include "Resource/ConstantBuffer.h"

#include "OtterEngine/Common/Math/MathUtils.h"
#include "OtterEngine/Common/External/ReadData.h"
#include "OtterEngine/Common/constants.h"

#pragma comment(lib, "d3d11.lib") // definitions of d3d11.h (D3D11CreateDeviceAndSwapChain)
#pragma comment(lib, "d3d12.lib") // definitions of d3d11.h (D3D11CreateDeviceAndSwapChain)

// IA = input assembler -> vertex buffer + index buffer
// OM = output merger

Graphics::Graphics(HWND hWnd, unsigned int viewportWidth, unsigned int viewportHeight) {

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

	// frame buffer create within this process
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
		DX::ThrowIfFailed(m_pDevice->CreateRenderTargetView(pBackBuffer.Get(), nullptr, &m_pRenderTargetView));

	// Initialize viewport
	m_viewport.Width = static_cast<float>(viewportWidth);
	m_viewport.Height = static_cast<float>(viewportHeight);
	m_viewport.MinDepth = 0.0f;
	m_viewport.MaxDepth = 1.0f;
	m_viewport.TopLeftX = 0.0f;
	m_viewport.TopLeftY = 0.0f;
	m_pDeviceContext->RSSetViewports(1u, &m_viewport);

	// z-buffer need to be created manually
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc = {};
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthStencilDesc.StencilEnable = false;	// skip all stencil stuff

	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> pDepthStencilState;
	m_pDevice->CreateDepthStencilState(&depthStencilDesc, &pDepthStencilState);

	m_pDeviceContext->OMSetDepthStencilState(pDepthStencilState.Get(), 1u);

	Microsoft::WRL::ComPtr<ID3D11Texture2D> pDepthStencil;
	D3D11_TEXTURE2D_DESC depthDesc = {};
	depthDesc.Width = kRenderWidth;		// should match with swap chain
	depthDesc.Height = kRenderHeight;	// should match with swap chain
	depthDesc.MipLevels = 1u;			// mip mapping 
	depthDesc.ArraySize = 1u;
	depthDesc.Format = DXGI_FORMAT_D32_FLOAT;
	depthDesc.SampleDesc.Count = 1u;	// for anti-aliasing
	depthDesc.SampleDesc.Quality = 0u;	// for anti-aliasing
	depthDesc.Usage = D3D11_USAGE_DEFAULT;
	depthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthDesc.CPUAccessFlags = 0u;
	depthDesc.MiscFlags = 0u;
	m_pDevice->CreateTexture2D(&depthDesc, nullptr, &pDepthStencil);

	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc= {};
	depthStencilViewDesc.Format = DXGI_FORMAT_D32_FLOAT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0u;
	m_pDevice->CreateDepthStencilView(pDepthStencil.Get(), &depthStencilViewDesc, &m_pDepthStencilView);

	// bind DepthStencilView to RenderTargetView only once
	m_pDeviceContext->OMSetRenderTargets(1u, m_pRenderTargetView.GetAddressOf(), m_pDepthStencilView.Get());

	ImGui_ImplDX11_Init(m_pDevice.Get(), m_pDeviceContext.Get());
}

void Graphics::ClearBuffer(float red, float green, float blue) {
	const float color[4] = {red, green, blue, 1.0f};

	m_pDeviceContext->ClearRenderTargetView(m_pRenderTargetView.Get(), color);
	m_pDeviceContext->ClearDepthStencilView(m_pDepthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0, 0u);
}

void Graphics::RenderIndexed(size_t indicesSize) const {
	m_pDeviceContext->DrawIndexed(static_cast<UINT>(indicesSize), 0u, 0u); // draw with index buffer
}

void Graphics::Update() {
}

void Graphics::PostUpdate() {
	m_pSwapChain->Present(1u, 0u); // swapchain present need to be call at the end of frame
}
