#pragma once

#include "GraphicsResource.h"

class PixelShader : public GraphicsResource
{
public:
	PixelShader(const Graphics& graphics);
	~PixelShader() = default;

	void Bind(const Graphics& graphics) const override;

private:
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pPixelShader;
	std::vector<uint8_t> m_pixelShaderBlob;
};
