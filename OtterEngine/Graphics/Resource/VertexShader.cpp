#include "VertexShader.h"

#include "OtterEngine/Common/External/ReadData.h"

VertexShader::VertexShader(const Graphics& graphics, const std::wstring& filename) : 
	GraphicsResource(GenerateUID(filename)) {

	m_vertexShaderBlob = DX::ReadData(filename.c_str());

	DX::ThrowIfFailed(GetDevice(graphics)->CreateVertexShader(m_vertexShaderBlob.data(), m_vertexShaderBlob.size(),
		nullptr, &m_pVertexShader));
}

void VertexShader::Bind(const Graphics& graphics) const {
	GetDeviceContext(graphics)->VSSetShader(m_pVertexShader.Get(), nullptr, 0);
}