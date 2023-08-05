#include "PixelShader.h"

#include "OtterEngine/Common/ReadData.h"

PixelShader::PixelShader(const Graphics& graphics) {

	m_pixelShaderBlob = DX::ReadData(L"PixelShader.cso");
	
	DX::ThrowIfFailed(GetDevice(graphics)->CreatePixelShader(m_pixelShaderBlob.data(), m_pixelShaderBlob.size(),
		nullptr, &m_pPixelShader));
}

void PixelShader::Bind(const Graphics& graphics) const {
	GetDeviceContext(graphics)->PSSetShader(m_pPixelShader.Get(), nullptr, 0);
}
