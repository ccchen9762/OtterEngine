#include "VertexShader.h"

#include "OtterEngine/Common/ReadData.h"

VertexShader::VertexShader(const Graphics& graphics) {

	m_vertexShaderBlob = DX::ReadData(L"VertexShader.cso");

	DX::ThrowIfFailed(GetDevice(graphics)->CreateVertexShader(m_vertexShaderBlob.data(), m_vertexShaderBlob.size(),
		nullptr, &m_pVertexShader));
}

void VertexShader::Bind(const Graphics& graphics) const {
	GetDeviceContext(graphics)->VSSetShader(m_pVertexShader.Get(), nullptr, 0);
}