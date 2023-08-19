#pragma once

#include "VertexShader.h"

class InputLayout : public GraphicsResource
{
public:
	enum class LayoutType {
		Basic, Texture, Phong
	};

public:
	InputLayout(const Graphics& graphics, const std::vector<uint8_t>& vertexShaderBlob, LayoutType type = LayoutType::Basic);
	~InputLayout() = default;

	void Bind(const Graphics& graphics) const override;

private:
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_pInputLayout;
	static const D3D11_INPUT_ELEMENT_DESC s_inputElementDesc[];
	static const D3D11_INPUT_ELEMENT_DESC s_inputElementDescTextured[];
	static const D3D11_INPUT_ELEMENT_DESC s_inputElementDescPhong[];
};