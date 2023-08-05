#include "PixelShader.h"

#include "OtterEngine/Common/ReadData.h"

PixelShader::PixelShader(const Microsoft::WRL::ComPtr<ID3D11Device>& pDevice) {

	m_pixelShaderBlob = DX::ReadData(L"PixelShader.cso");
	
	DX::ThrowIfFailed(pDevice->CreatePixelShader(m_pixelShaderBlob.data(), m_pixelShaderBlob.size(),
		nullptr, &m_pPixelShader));
}

void PixelShader::Bind(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContext) const {
	pDeviceContext->PSSetShader(m_pPixelShader.Get(), nullptr, 0);
}
