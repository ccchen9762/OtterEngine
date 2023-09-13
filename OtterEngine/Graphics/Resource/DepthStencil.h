#pragma once

#include "GraphicsResource.h"
#include "RenderTarget.h"

class DepthStencil : public GraphicsResource
{
	friend class RenderTarget;

public:
	DepthStencil(const Graphics& graphics, unsigned int width, unsigned int height);
	~DepthStencil() = default;

	void Bind(const Graphics& graphics) const override;
	void Clear(const Graphics& graphics);

private:
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_pDepthStencilView;
};