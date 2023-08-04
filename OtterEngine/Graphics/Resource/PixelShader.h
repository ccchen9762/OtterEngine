#pragma once

#include "GraphicsResource.h"

class PixelShader : public GraphicsResource
{
public:
	PixelShader(const Microsoft::WRL::ComPtr<ID3D11Device>& pDevice);
	~PixelShader() = default;

	void Bind(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContext) const override;

private:
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pPixelShader;
	std::vector<uint8_t> m_pixelShaderBlob;
};
