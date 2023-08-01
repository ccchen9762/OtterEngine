#pragma once

#include "VertexShader.h"

class InputLayout : public GraphicsResource
{
public:
	InputLayout(ID3D11Device* pDevice, const std::vector<uint8_t>& vertexShaderBlob);
	~InputLayout() = default;

	void Bind(ID3D11DeviceContext* pDeviceContext) const;

private:
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_pInputLayout;
};