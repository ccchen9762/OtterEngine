#include "Graphics.h"

#include "OtterEngine/Math/Vector3.h"
#include "OtterEngine/Common/ReadData.h"
#include <DirectXMath.h>


#pragma comment(lib, "d3d11.lib")		// definitions

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

	// Initialize viewport
	viewport.Width = viewportWidth;
	viewport.Height = viewportHeight;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;
	m_pDeviceContext->RSSetViewports(1u, &viewport);
}

void Graphics::ClearBuffer(float red, float green, float blue) {
	const float color[4] = {red, green, blue, 1.0f};

	m_pDeviceContext->ClearRenderTargetView(m_pRenderTarget.Get(), color);
}

void Graphics::Update() {

	DrawTriangle();
	PostUpdate();
}

void Graphics::PostUpdate() {
	m_pSwapChain->Present(1u, 0u); // swapchain present need to be call at the end of frame
}

void Graphics::DrawTriangle() {

	/* Loadand create shaders. */
	// vertex shader
	Microsoft::WRL::ComPtr<ID3D11VertexShader> pVertexShader;
	std::vector<uint8_t> vertexShaderBlob = DX::ReadData(L"VertexShader.cso");
	DX::ThrowIfFailed(m_pDevice->CreateVertexShader(vertexShaderBlob.data(), vertexShaderBlob.size(),
		nullptr, &pVertexShader));

	// pixel shader
	Microsoft::WRL::ComPtr<ID3D11PixelShader> pPixelShader;
	std::vector<uint8_t> pixelShaderBlob = DX::ReadData(L"PixelShader.cso");
	DX::ThrowIfFailed(m_pDevice->CreatePixelShader(pixelShaderBlob.data(), pixelShaderBlob.size(),
		nullptr, &pPixelShader));

	
	/* input layout */
	Microsoft::WRL::ComPtr<ID3D11InputLayout> pInputLayout;
	const D3D11_INPUT_ELEMENT_DESC s_inputElementDesc[] = {
		{ "SV_Position", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	DX::ThrowIfFailed(m_pDevice->CreateInputLayout(s_inputElementDesc, 1,
		vertexShaderBlob.data(), vertexShaderBlob.size(), &pInputLayout));
	
	/* IA(Input assembler)->vertex buffer + index buffer */
	
	//vertex buffer
	const DirectX::XMFLOAT4 vertices[] = {
		DirectX::XMFLOAT4(0.0f,   0.5f,  -1.0f, 1.0f),
		DirectX::XMFLOAT4(0.5f,  -0.5f,  -3.0f, 1.0f),
		DirectX::XMFLOAT4(-0.5f, -0.5f,  0.5f, 1.0f) 
	};

	
	Microsoft::WRL::ComPtr<ID3D11Buffer> pVertexBuffer;

	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.ByteWidth = sizeof(vertices); // return total array size in bytes
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0u;
	bufferDesc.MiscFlags = 0u;
	bufferDesc.StructureByteStride = sizeof(DirectX::XMFLOAT4);

	D3D11_SUBRESOURCE_DATA subResourceData = {};
	subResourceData.pSysMem = vertices;

	DX::ThrowIfFailed(m_pDevice->CreateBuffer(&bufferDesc, &subResourceData, &pVertexBuffer));

	m_pDeviceContext->OMSetRenderTargets(1u, m_pRenderTarget.GetAddressOf(), nullptr);

	m_pDeviceContext->IASetInputLayout(pInputLayout.Get());
	
	const unsigned int stride = sizeof(DirectX::XMFLOAT4);
	const unsigned int offset = 0u;

	// pOffsets -> Each offset is the bytes between the first element of a vertex buffer and the first element that will be used.
	m_pDeviceContext->IASetVertexBuffers(0u, 1u, pVertexBuffer.GetAddressOf(), &stride, &offset);
	m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// bind shaders
	m_pDeviceContext->VSSetShader(pVertexShader.Get(), nullptr, 0);
	m_pDeviceContext->PSSetShader(pPixelShader.Get(), nullptr, 0);

	m_pDeviceContext->Draw(3u, 0u);
}
