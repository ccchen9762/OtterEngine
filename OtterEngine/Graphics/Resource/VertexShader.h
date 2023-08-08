#pragma once

#include "GraphicsResource.h"

class VertexShader : public GraphicsResource
{
public:
	VertexShader(const Graphics& graphics, const std::wstring& filename);
	~VertexShader() = default;

	void Bind(const Graphics& graphics) const override;

	const std::vector<uint8_t>& GetVertexShaderBlob() const { return m_vertexShaderBlob; }

private:
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_pVertexShader;
	std::vector<uint8_t> m_vertexShaderBlob;
};