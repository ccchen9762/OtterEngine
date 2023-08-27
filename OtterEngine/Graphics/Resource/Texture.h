#pragma once

#include "GraphicsResource.h"

class Texture : public GraphicsResource
{
public:
	Texture(const Graphics& graphics, const std::wstring& path, unsigned int slot);
	~Texture() = default;

	static std::wstring GenerateUID(const std::wstring& path, unsigned int slot) { return L"Texture#" + path + L"#" + std::to_wstring(slot); }

	void Bind(const Graphics& graphics) const override;
	void UnBind(const Graphics& graphics) const;

private:
	unsigned int m_slot;
	std::vector<uint8_t> m_textureBuffer;
	unsigned int m_textureWidth, m_textureHeight;
	Microsoft::WRL::ComPtr<ID3D11SamplerState> m_pSampler;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_pTextureView;
}; 