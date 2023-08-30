#include "Texture.h"

#include "OtterEngine/Common/External/FindMedia.h"
#include "OtterEngine/Common/External/LoadImage.h"

Texture::Texture(const Graphics& graphics, const std::wstring& path, Texture::Type slot) :
	GraphicsResource(GenerateUID(path,slot)), m_slot(static_cast<unsigned int>(slot)) {

	if (!path.empty()) {
		D3D11_SAMPLER_DESC samplerDesc = {};
		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		DX::ThrowIfFailed(GetDevice(graphics)->CreateSamplerState(&samplerDesc, &m_pSampler));

		wchar_t buff[MAX_PATH];
		DX::FindMediaFile(buff, MAX_PATH, path.c_str());
		m_textureBuffer = LoadBGRAImage(buff, m_textureWidth, m_textureHeight);

		D3D11_TEXTURE2D_DESC textureDesc = {};
		textureDesc.Width = m_textureWidth;
		textureDesc.Height = m_textureHeight;
		textureDesc.MipLevels = 1;
		textureDesc.ArraySize = 1;
		textureDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.SampleDesc.Quality = 0;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		textureDesc.CPUAccessFlags = 0;
		textureDesc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA textureSubResourceData = {};
		textureSubResourceData.pSysMem = m_textureBuffer.data();
		textureSubResourceData.SysMemPitch = textureDesc.Width * sizeof(uint32_t);

		Microsoft::WRL::ComPtr<ID3D11Texture2D> pTexture;
		DX::ThrowIfFailed(GetDevice(graphics)->CreateTexture2D(&textureDesc, &textureSubResourceData, &pTexture));

		D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc = {};
		shaderResourceViewDesc.Format = textureDesc.Format;
		shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
		shaderResourceViewDesc.Texture2D.MipLevels = 1;
		DX::ThrowIfFailed(GetDevice(graphics)->CreateShaderResourceView(pTexture.Get(), &shaderResourceViewDesc, &m_pTextureView));
	}
	else { // for model parts that don't have specific texture
		m_pSampler = { nullptr };
		m_pTextureView = { nullptr };
	}
}

void Texture::Bind(const Graphics& graphics) const {
	GetDeviceContext(graphics)->PSSetSamplers(m_slot, 1u, m_pSampler.GetAddressOf());
	GetDeviceContext(graphics)->PSSetShaderResources(m_slot, 1u, m_pTextureView.GetAddressOf());
}

void Texture::UnBind(const Graphics& graphics) const {
	Microsoft::WRL::ComPtr<ID3D11SamplerState> nullSampler[1] = { nullptr };
	GetDeviceContext(graphics)->PSSetSamplers(m_slot, 1, nullSampler->GetAddressOf());
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> nullShaderResourceView[1] = { nullptr };
	GetDeviceContext(graphics)->PSSetShaderResources(m_slot, 1, nullShaderResourceView->GetAddressOf());
}