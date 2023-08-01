#include "PixelShader.h"

#include "OtterEngine/Common/ReadData.h"

#pragma comment(lib, "d3d11.lib")

PixelShader::PixelShader(ID3D11Device* pDevice) {

	m_pixelShaderBlob = DX::ReadData(L"PixelShader.cso");
	
	DX::ThrowIfFailed(pDevice->CreatePixelShader(m_pixelShaderBlob.data(), m_pixelShaderBlob.size(),
		nullptr, &m_pPixelShader));
}

void PixelShader::Bind(ID3D11DeviceContext* pDeviceContext) const {
	pDeviceContext->PSSetShader(m_pPixelShader.Get(), nullptr, 0);
}
