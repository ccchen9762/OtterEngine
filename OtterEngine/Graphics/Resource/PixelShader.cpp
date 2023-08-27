#include "PixelShader.h"

#include "OtterEngine/Common/External/ReadData.h"

PixelShader::PixelShader(const Graphics& graphics, const std::wstring& filename) :
	GraphicsResource(GenerateUID(filename)) {

	m_pixelShaderBlob = DX::ReadData(filename.c_str());
	
	DX::ThrowIfFailed(GetDevice(graphics)->CreatePixelShader(m_pixelShaderBlob.data(), m_pixelShaderBlob.size(),
		nullptr, &m_pPixelShader));
}

void PixelShader::Bind(const Graphics& graphics) const {
	GetDeviceContext(graphics)->PSSetShader(m_pPixelShader.Get(), nullptr, 0);
}