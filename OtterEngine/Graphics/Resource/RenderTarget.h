#pragma once

#include "GraphicsResource.h"

class DepthStencil;

class RenderTarget: public GraphicsResource
{
public:
	RenderTarget(const Graphics& graphics, unsigned int width, unsigned int height);
	RenderTarget(const Graphics& graphics, ID3D11Texture2D* pBackBuffer);
	~RenderTarget() = default;

	void BindAsTexture(const Graphics& graphics) const;
	void BindAsTargetView(const Graphics& graphics) const;
	void BindAsTargetView(const Graphics& graphics, const DepthStencil& depthStencil) const;

private:
	unsigned int m_width, m_height;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_pShaderResourceView;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_pRenderTargetView;
};