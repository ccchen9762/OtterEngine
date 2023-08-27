#pragma once

#include "GraphicsResource.h"

class PixelShader : public GraphicsResource
{
public:
	PixelShader(const Graphics& graphics, const std::wstring& filename);
	~PixelShader() = default;

	static std::wstring GenerateUID(const std::wstring& filename) { return L"PixelShader#" + filename; }

	void Bind(const Graphics& graphics) const override;

private:
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pPixelShader;
	std::vector<uint8_t> m_pixelShaderBlob;
};
