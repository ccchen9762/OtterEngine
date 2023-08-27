#pragma once

#include "GraphicsResource.h"

class InputLayout : public GraphicsResource
{
public:
	enum class LayoutType {
		Basic, Texture, Shading, TextureShading
	};

public:
	InputLayout(const Graphics& graphics, const std::vector<uint8_t>& vertexShaderBlob, LayoutType type = LayoutType::Basic);
	~InputLayout() = default;

	static std::wstring GenerateUID(const std::vector<uint8_t>& vertexShaderBlob, LayoutType type = LayoutType::Basic) { 
		return L"InputLayout#" + std::to_wstring(static_cast<int>(type)); 
	}

	void Bind(const Graphics& graphics) const override;

private:
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_pInputLayout;
	static const D3D11_INPUT_ELEMENT_DESC s_inputElementDesc[];
	static const D3D11_INPUT_ELEMENT_DESC s_inputElementDescTextured[];
	static const D3D11_INPUT_ELEMENT_DESC s_inputElementDescShading[];
	static const D3D11_INPUT_ELEMENT_DESC s_inputElementDescTexturedShading[];
};