#include "RenderTarget.h"

#include "Texture.h"
#include "DepthStencil.h"

RenderTarget::RenderTarget(const Graphics& graphics, unsigned int width, unsigned int height) : 
	GraphicsResource(L""), m_width(width), m_height(height) {
	
	D3D11_TEXTURE2D_DESC textureDesc = {};
	textureDesc.Width = width;
	textureDesc.Height = height;
	textureDesc.MipLevels = 1u; 
	textureDesc.ArraySize = 1u;
	textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	textureDesc.SampleDesc.Count = 1u;	// for anti-aliasing
	textureDesc.SampleDesc.Quality = 0u;	// for anti-aliasing
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0u;
	textureDesc.MiscFlags = 0u;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> pTexture;
	GetDevice(graphics)->CreateTexture2D(&textureDesc, nullptr, &pTexture);

	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc = {};
	shaderResourceViewDesc.Format = textureDesc.Format;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	shaderResourceViewDesc.Texture2D.MipLevels = 1;
	DX::ThrowIfFailed(GetDevice(graphics)->CreateShaderResourceView(pTexture.Get(), &shaderResourceViewDesc, &m_pShaderResourceView));

	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc = {};
	renderTargetViewDesc.Format = textureDesc.Format;
	renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	renderTargetViewDesc.Texture2D = D3D11_TEX2D_RTV{ 0 };
	DX::ThrowIfFailed(GetDevice(graphics)->CreateRenderTargetView(pTexture.Get(), &renderTargetViewDesc, &m_pRenderTargetView));
}

RenderTarget::RenderTarget(const Graphics& graphics, ID3D11Texture2D* pTexture) :
	GraphicsResource(L"") {
	
	D3D11_TEXTURE2D_DESC textureDesc;
	pTexture->GetDesc(&textureDesc);
	m_width = textureDesc.Width;
	m_height = textureDesc.Height;

	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc = {};
	renderTargetViewDesc.Format = textureDesc.Format;
	renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	renderTargetViewDesc.Texture2D = D3D11_TEX2D_RTV{ 0 };
	DX::ThrowIfFailed(GetDevice(graphics)->CreateRenderTargetView(pTexture, &renderTargetViewDesc, &m_pRenderTargetView));
}

void RenderTarget::BindAsTexture(const Graphics& graphics) const {
	GetDeviceContext(graphics)->PSSetShaderResources(static_cast<unsigned int>(Texture::Type::RenderTarget), 1u, m_pShaderResourceView.GetAddressOf());
}

void RenderTarget::BindAsTargetView(const Graphics& graphics) const {
	GetDeviceContext(graphics)->OMSetRenderTargets(1u, m_pRenderTargetView.GetAddressOf(), nullptr);

	// Initialize viewport
	D3D11_VIEWPORT viewport;
	viewport.Width = m_width;
	viewport.Height = m_height;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;
	GetDeviceContext(graphics)->RSSetViewports(1u, &viewport);
}

void RenderTarget::BindAsTargetView(const Graphics& graphics, const DepthStencil& depthStencil) const {
	GetDeviceContext(graphics)->OMSetRenderTargets(1u, m_pRenderTargetView.GetAddressOf(), depthStencil.m_pDepthStencilView.Get());

	// Initialize viewport
	D3D11_VIEWPORT viewport;
	viewport.Width = m_width;
	viewport.Height = m_height;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;
	GetDeviceContext(graphics)->RSSetViewports(1u, &viewport);
}
