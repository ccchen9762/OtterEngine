#include "Texture.h"

#include "OtterEngine/Common/External/FindMedia.h"
#include "OtterEngine/Common/External/LoadImage.h"

Texture::Texture(const Graphics& graphics, const std::wstring& path, Texture::Type slot) :
	GraphicsResource(GenerateUID(path,slot)), m_slot(static_cast<unsigned int>(slot)) {

	if (!path.empty()) {
		D3D11_SAMPLER_DESC samplerDesc = {};
		//samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
		samplerDesc.MaxAnisotropy = D3D11_MAX_MAXANISOTROPY; // max level, cost more GPU resource
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.MipLODBias = 0.0f;
		samplerDesc.MinLOD = 0.0f;
		samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
		DX::ThrowIfFailed(GetDevice(graphics)->CreateSamplerState(&samplerDesc, &m_pSampler));

		wchar_t buff[MAX_PATH];
		DX::FindMediaFile(buff, MAX_PATH, path.c_str());
		m_textureBuffer = LoadBGRAImage(buff, m_textureWidth, m_textureHeight);

		D3D11_TEXTURE2D_DESC textureDesc = {};
		textureDesc.Width = m_textureWidth;
		textureDesc.Height = m_textureHeight;
		textureDesc.MipLevels = 0; // mipmap all the way down to 1x1  
		textureDesc.ArraySize = 1;
		textureDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.SampleDesc.Quality = 0;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | 
								D3D11_BIND_RENDER_TARGET; // for GPU to write back to it
		textureDesc.CPUAccessFlags = 0;
		textureDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

		D3D11_SUBRESOURCE_DATA textureSubResourceData = {};
		textureSubResourceData.pSysMem = m_textureBuffer.data();
		textureSubResourceData.SysMemPitch = textureDesc.Width * sizeof(uint32_t);

		Microsoft::WRL::ComPtr<ID3D11Texture2D> pTexture;
		DX::ThrowIfFailed(GetDevice(graphics)->CreateTexture2D(&textureDesc, nullptr, &pTexture));
		GetDeviceContext(graphics)->UpdateSubresource(pTexture.Get(), 0u, nullptr, m_textureBuffer.data(), textureDesc.Width * sizeof(uint32_t), 0u);

		D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc = {};
		shaderResourceViewDesc.Format = textureDesc.Format;
		shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
		shaderResourceViewDesc.Texture2D.MipLevels = -1; // let system figure out
		DX::ThrowIfFailed(GetDevice(graphics)->CreateShaderResourceView(pTexture.Get(), &shaderResourceViewDesc, &m_pTextureView));

		GetDeviceContext(graphics)->GenerateMips(m_pTextureView.Get());
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