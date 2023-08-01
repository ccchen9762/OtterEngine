#include "VertexShader.h"

#include "OtterEngine/Common/ReadData.h"

#pragma comment(lib, "d3d11.lib")

VertexShader::VertexShader(ID3D11Device* pDevice) {

	m_vertexShaderBlob = DX::ReadData(L"VertexShader.cso");

	DX::ThrowIfFailed(pDevice->CreateVertexShader(m_vertexShaderBlob.data(), m_vertexShaderBlob.size(),
		nullptr, &m_pVertexShader));
}

void VertexShader::Bind(ID3D11DeviceContext* pDeviceContext) const {
	pDeviceContext->VSSetShader(m_pVertexShader.Get(), nullptr, 0);
}