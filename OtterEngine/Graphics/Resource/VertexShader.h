#pragma once

#include "GraphicsResource.h"

class VertexShader : public GraphicsResource
{
public:
	VertexShader(const Graphics& graphics, const std::wstring& filename);
	~VertexShader() = default;

	static std::wstring GenerateUID(const std::wstring& filename) { return L"VertexShader#" + filename; }
	
	const std::vector<uint8_t>& GetVertexShaderBlob() const { return m_vertexShaderBlob; }

	void Bind(const Graphics& graphics) const override;

private:
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_pVertexShader;
	std::vector<uint8_t> m_vertexShaderBlob;
};