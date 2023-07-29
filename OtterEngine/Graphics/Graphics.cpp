#include "Graphics.h"

#include <DirectXMath.h>

#include "OtterEngine/Math/Vector3.h"
#include "OtterEngine/Common/ReadData.h"
#include "OtterEngine/Common/constants.h"

#pragma comment(lib, "d3d11.lib")		// definitions of d3d11.h

// IA = input assembler
// OM = output merger

struct Vertex {
	DirectX::XMVECTOR m_position;
	DirectX::XMFLOAT4 m_color;
};

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
	m_viewport.Width = viewportWidth;
	m_viewport.Height = viewportHeight;
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
}

void Graphics::ClearBuffer(float red, float green, float blue) {
	const float color[4] = {red, green, blue, 1.0f};

	m_pDeviceContext->ClearRenderTargetView(m_pRenderTargetView.Get(), color);
	m_pDeviceContext->ClearDepthStencilView(m_pDepthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0, 0u);
}

void Graphics::Update() {
}

void Graphics::PostUpdate() {
	m_pSwapChain->Present(1u, 0u); // swapchain present need to be call at the end of frame
}

void Graphics::CreateRenderResource() {
	/* Loadand create shaders */
	// vertex shader
	std::vector<uint8_t> vertexShaderBlob = DX::ReadData(L"VertexShader.cso");
	DX::ThrowIfFailed(m_pDevice->CreateVertexShader(vertexShaderBlob.data(), vertexShaderBlob.size(),
		nullptr, &m_pVertexShader));

	// pixel shader
	std::vector<uint8_t> pixelShaderBlob = DX::ReadData(L"PixelShader.cso");
	DX::ThrowIfFailed(m_pDevice->CreatePixelShader(pixelShaderBlob.data(), pixelShaderBlob.size(),
		nullptr, &m_pPixelShader));

	// input layout
	// AlignedByteOffset can use D3D11_APPEND_ALIGNED_ELEMENT to automatically assign
	const D3D11_INPUT_ELEMENT_DESC s_inputElementDesc[] = {
		{ "SV_Position", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0,   D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",		 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16u, D3D11_INPUT_PER_VERTEX_DATA, 0 }, // semantic takes COLOR instead of COLOR0 ?
	};
	DX::ThrowIfFailed(m_pDevice->CreateInputLayout(s_inputElementDesc, sizeof(s_inputElementDesc) / sizeof(D3D11_INPUT_ELEMENT_DESC),
		vertexShaderBlob.data(), vertexShaderBlob.size(), &m_pInputLayout));
}

void Graphics::DrawTriangle(double angle) {
	/* IA(Input assembler)->vertex buffer + index buffer */
	// ==================== create vertex buffer ====================
	const Vertex vertices[] = {
		{DirectX::XMVectorSet(0.0f,  0.5f,  0.0f, 1.0f), DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f)},
		{DirectX::XMVectorSet(0.5f, -0.5f,  0.0f, 1.0f), DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f)},
		{DirectX::XMVectorSet(-0.5f, -0.5f,  0.0f, 1.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f)},
	};
	Microsoft::WRL::ComPtr<ID3D11Buffer> pVertexBuffer;
	D3D11_BUFFER_DESC vertexBufferDesc = {};
	vertexBufferDesc.ByteWidth = sizeof(vertices); // return total array size in bytes
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0u;
	vertexBufferDesc.MiscFlags = 0u;
	vertexBufferDesc.StructureByteStride = sizeof(Vertex);
	D3D11_SUBRESOURCE_DATA vertexSubResourceData = {};
	vertexSubResourceData.pSysMem = vertices;
	DX::ThrowIfFailed(m_pDevice->CreateBuffer(&vertexBufferDesc, &vertexSubResourceData, &pVertexBuffer));

	// ==================== create index buffer ====================
	// indices are 2 bytes by default
	unsigned short indices[] = {
		0,1,2
	};
	Microsoft::WRL::ComPtr<ID3D11Buffer> pIndexBuffer;
	D3D11_BUFFER_DESC indexBufferDesc = {};
	indexBufferDesc.ByteWidth = sizeof(indices); // return total array size in bytes
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0u;
	indexBufferDesc.MiscFlags = 0u;
	indexBufferDesc.StructureByteStride = sizeof(unsigned short);
	D3D11_SUBRESOURCE_DATA indexSubResourceData = {};
	indexSubResourceData.pSysMem = indices;
	DX::ThrowIfFailed(m_pDevice->CreateBuffer(&indexBufferDesc, &indexSubResourceData, &pIndexBuffer));

	// VS -> constant buffer
	// ==================== create constant buffer ====================
	struct ConstantBuffer {
		DirectX::XMMATRIX transform;
	};

	const ConstantBuffer constantbuffer = {
		DirectX::XMMatrixTranspose(
			DirectX::XMMatrixRotationZ(angle) *
			DirectX::XMMatrixScaling(kRenderRatio, 1.0, 1.0)  // for cancel default normalized device coordinate
		)
	};
	Microsoft::WRL::ComPtr<ID3D11Buffer> pConstantBuffer;
	D3D11_BUFFER_DESC constantBufferDesc = {};
	constantBufferDesc.ByteWidth = sizeof(constantbuffer); // return total array size in bytes
	constantBufferDesc.Usage = D3D11_USAGE_DYNAMIC;	// to be able to update every frame
	constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constantBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; // cpu need permission to update buffer
	constantBufferDesc.MiscFlags = 0u;
	constantBufferDesc.StructureByteStride = 0u; // only single element
	D3D11_SUBRESOURCE_DATA constantSubResourceData = {};
	constantSubResourceData.pSysMem = &constantbuffer;
	DX::ThrowIfFailed(m_pDevice->CreateBuffer(&constantBufferDesc, &constantSubResourceData, &pConstantBuffer));

	// ==================== bind buffers ====================
	const unsigned int vertexBufferStride = sizeof(Vertex);
	const unsigned int vertexBufferoffset = 0u;

	// pOffsets -> Each offset is the bytes between the first element of a vertex buffer and the first element that will be used.
	m_pDeviceContext->IASetVertexBuffers(0u, 1u, pVertexBuffer.GetAddressOf(), &vertexBufferStride, &vertexBufferoffset);
	m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_pDeviceContext->IASetIndexBuffer(pIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0u);
	m_pDeviceContext->VSSetConstantBuffers(0u, 1u, pConstantBuffer.GetAddressOf());

	// ==================== bind device dependent resource ====================
	m_pDeviceContext->IASetInputLayout(m_pInputLayout.Get());
	m_pDeviceContext->VSSetShader(m_pVertexShader.Get(), nullptr, 0);
	m_pDeviceContext->PSSetShader(m_pPixelShader.Get(), nullptr, 0);

	// ==================== draw call ====================
	//m_pDeviceContext->Draw(3u, 0u);
	m_pDeviceContext->DrawIndexed(sizeof(indices) / sizeof(unsigned short), 0u, 0u); // draw with index buffer
}

void Graphics::DrawCube(double angleX, double angleY, double angleZ) {
	/* IA(Input assembler)->vertex buffer + index buffer */
	// ==================== create vertex buffer ====================
	const Vertex vertices[] = {
		{DirectX::XMVectorSet( -1.0f, -1.0f, -1.0f, 1.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f)},
		{DirectX::XMVectorSet(  1.0f, -1.0f, -1.0f, 1.0f), DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f)},
		{DirectX::XMVectorSet(  1.0f,  1.0f, -1.0f, 1.0f), DirectX::XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f)},
		{DirectX::XMVectorSet( -1.0f,  1.0f, -1.0f, 1.0f), DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f)},
		{DirectX::XMVectorSet( -1.0f, -1.0f,  1.0f, 1.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f)},
		{DirectX::XMVectorSet(  1.0f, -1.0f,  1.0f, 1.0f), DirectX::XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f)},
		{DirectX::XMVectorSet(  1.0f,  1.0f,  1.0f, 1.0f), DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f)},
		{DirectX::XMVectorSet( -1.0f,  1.0f,  1.0f, 1.0f), DirectX::XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f)},
	};
	Microsoft::WRL::ComPtr<ID3D11Buffer> pVertexBuffer;
	D3D11_BUFFER_DESC vertexBufferDesc = {};
	vertexBufferDesc.ByteWidth = sizeof(vertices); // return total array size in bytes
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0u;
	vertexBufferDesc.MiscFlags = 0u;
	vertexBufferDesc.StructureByteStride = sizeof(Vertex);
	D3D11_SUBRESOURCE_DATA vertexSubResourceData = {};
	vertexSubResourceData.pSysMem = vertices;
	DX::ThrowIfFailed(m_pDevice->CreateBuffer(&vertexBufferDesc, &vertexSubResourceData, &pVertexBuffer));

	// ==================== create index buffer ====================
	// indices are 2 bytes by default
	const unsigned short indices[] = {
		3, 0, 1,  1, 2, 3, // back
		6, 5, 4,  4, 7, 6, // front
		7, 4, 0,  0, 3, 7, // left
		6, 5, 1,  1, 2, 6, // right
		2, 6, 7,  7, 3, 2, // top
		4, 5, 1,  1, 0, 4, // bottom
	};
	Microsoft::WRL::ComPtr<ID3D11Buffer> pIndexBuffer;
	D3D11_BUFFER_DESC indexBufferDesc = {};
	indexBufferDesc.ByteWidth = sizeof(indices); // return total array size in bytes
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0u;
	indexBufferDesc.MiscFlags = 0u;
	indexBufferDesc.StructureByteStride = sizeof(unsigned short);
	D3D11_SUBRESOURCE_DATA indexSubResourceData = {};
	indexSubResourceData.pSysMem = indices;
	DX::ThrowIfFailed(m_pDevice->CreateBuffer(&indexBufferDesc, &indexSubResourceData, &pIndexBuffer));

	// VS -> constant buffer
	// ==================== create constant buffer ====================
	struct ConstantBuffer {
		DirectX::XMMATRIX transform;
	};

	// in Left Hand System (LH) Z axis direction is away from screen, RH toward screen **
	const ConstantBuffer constantbuffer = {
		DirectX::XMMatrixTranspose(
			DirectX::XMMatrixRotationX(angleX) *
			DirectX::XMMatrixRotationY(angleX) *
			DirectX::XMMatrixRotationZ(angleZ) *
			DirectX::XMMatrixTranslation(0.0f,  0.0f, -4.0f) *
			DirectX::XMMatrixPerspectiveRH(1.0, kRenderRatio, 0.1f, 10.0f)
		)
	};
	Microsoft::WRL::ComPtr<ID3D11Buffer> pConstantBuffer;
	D3D11_BUFFER_DESC constantBufferDesc = {};
	constantBufferDesc.ByteWidth = sizeof(constantbuffer); // return total array size in bytes
	constantBufferDesc.Usage = D3D11_USAGE_DYNAMIC;	// to be able to update every frame
	constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constantBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; // cpu need permission to update buffer
	constantBufferDesc.MiscFlags = 0u;
	constantBufferDesc.StructureByteStride = 0u; // only single element
	D3D11_SUBRESOURCE_DATA constantSubResourceData = {};
	constantSubResourceData.pSysMem = &constantbuffer;
	DX::ThrowIfFailed(m_pDevice->CreateBuffer(&constantBufferDesc, &constantSubResourceData, &pConstantBuffer));

	// ==================== bind buffers ====================
	const unsigned int vertexBufferStride = sizeof(Vertex);
	const unsigned int vertexBufferoffset = 0u;

	// pOffsets -> Each offset is the bytes between the first element of a vertex buffer and the first element that will be used.
	m_pDeviceContext->IASetVertexBuffers(0u, 1u, pVertexBuffer.GetAddressOf(), &vertexBufferStride, &vertexBufferoffset);
	m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_pDeviceContext->IASetIndexBuffer(pIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0u);
	m_pDeviceContext->VSSetConstantBuffers(0u, 1u, pConstantBuffer.GetAddressOf());

	// ==================== bind device dependent resource ====================
	m_pDeviceContext->IASetInputLayout(m_pInputLayout.Get());
	m_pDeviceContext->VSSetShader(m_pVertexShader.Get(), nullptr, 0);
	m_pDeviceContext->PSSetShader(m_pPixelShader.Get(), nullptr, 0);

	// ==================== draw call ====================
	//m_pDeviceContext->Draw(3u, 0u);
	m_pDeviceContext->DrawIndexed(sizeof(indices) / sizeof(unsigned short), 0u, 0u); // draw with index buffer
}
