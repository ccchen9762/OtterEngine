#include "DepthStencil.h"

DepthStencil::DepthStencil(const Graphics& graphics, unsigned int width, unsigned int height) : GraphicsResource(L"") {
	
	Microsoft::WRL::ComPtr<ID3D11Texture2D> pDepthStencil;
	D3D11_TEXTURE2D_DESC depthDesc = {};
	depthDesc.Width = width;		// should match with swap chain
	depthDesc.Height = height;	// should match with swap chain
	depthDesc.MipLevels = 1u;			// mip mapping 
	depthDesc.ArraySize = 1u;
	depthDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT; // DXGI_FORMAT_D32_FLOAT
	depthDesc.SampleDesc.Count = 1u;	// for anti-aliasing
	depthDesc.SampleDesc.Quality = 0u;	// for anti-aliasing
	depthDesc.Usage = D3D11_USAGE_DEFAULT;
	depthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthDesc.CPUAccessFlags = 0u;
	depthDesc.MiscFlags = 0u;
	GetDevice(graphics)->CreateTexture2D(&depthDesc, nullptr, &pDepthStencil);

	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc = {};
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT; // DXGI_FORMAT_D32_FLOAT
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0u;
	GetDevice(graphics)->CreateDepthStencilView(pDepthStencil.Get(), nullptr, &m_pDepthStencilView);
}

void DepthStencil::Bind(const Graphics& graphics) const {
	GetDeviceContext(graphics)->OMSetRenderTargets(0, nullptr, m_pDepthStencilView.Get());
}

void DepthStencil::Clear(const Graphics& graphics) {
	GetDeviceContext(graphics)->ClearDepthStencilView(m_pDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0u);
}
