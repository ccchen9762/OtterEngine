#pragma once

#include "GraphicsResource.h"

class VertexShader : public GraphicsResource
{
public:
	VertexShader(ID3D11Device* pDevice);
	~VertexShader() = default;

	void Bind(ID3D11DeviceContext* pDeviceContext) const;

	const std::vector<uint8_t>& GetVertexShaderBlob() const { return m_vertexShaderBlob; }

private:
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_pVertexShader;
	std::vector<uint8_t> m_vertexShaderBlob;
};