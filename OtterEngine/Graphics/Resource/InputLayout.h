#pragma once

#include "VertexShader.h"

class InputLayout : public GraphicsResource
{
public:
	InputLayout(const Microsoft::WRL::ComPtr<ID3D11Device>& pDevice, const std::vector<uint8_t>& vertexShaderBlob);
	~InputLayout() = default;

	void Bind(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContext) const override;

private:
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_pInputLayout;
};