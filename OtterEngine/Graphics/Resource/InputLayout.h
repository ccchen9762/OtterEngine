#pragma once

#include "VertexShader.h"

class InputLayout : public GraphicsResource
{
public:
	InputLayout(const Graphics& graphics, const std::vector<uint8_t>& vertexShaderBlob);
	~InputLayout() = default;

	void Bind(const Graphics& graphics) const override;

private:
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_pInputLayout;
};