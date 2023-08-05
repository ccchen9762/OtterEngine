#include "ConstantBuffer.h"

#include "OtterEngine/Common/constants.h"

ConstantBufferVertex::ConstantBufferVertex(const Microsoft::WRL::ComPtr<ID3D11Device>& pDevice, const Entity& parentEntity)
	: m_parentEntity(parentEntity) {

	// in Left Hand System (LH) Z axis direction is away from screen, RH toward screen
	// remember XMMatrixTranspose only need once
	const CBuffer cBuffer = {
		DirectX::XMMatrixTranspose(
			m_parentEntity.GetTransformMatrix() *
			DirectX::XMMatrixPerspectiveFovRH(DirectX::XM_PIDIV4, kRenderRatio, 0.1f, 100.0f)
		)
	};

	D3D11_BUFFER_DESC constantBufferDesc = {};
	constantBufferDesc.ByteWidth = sizeof(cBuffer); // return total array size in bytes
	constantBufferDesc.Usage = D3D11_USAGE_DYNAMIC;	// to be able to update every frame
	constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constantBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; // cpu need permission to update buffer
	constantBufferDesc.MiscFlags = 0u;
	constantBufferDesc.StructureByteStride = 0u; // only single element
	D3D11_SUBRESOURCE_DATA constantSubResourceData = {};
	constantSubResourceData.pSysMem = &cBuffer;
	DX::ThrowIfFailed(pDevice->CreateBuffer(&constantBufferDesc, &constantSubResourceData, &m_pConstantBuffer));
}

void ConstantBufferVertex::Bind(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContext) const {
	Update(pDeviceContext);
	pDeviceContext->VSSetConstantBuffers(0u, 1u, m_pConstantBuffer.GetAddressOf());
}

void ConstantBufferVertex::Update(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContext) const {

	const CBuffer cBuffer = {
		DirectX::XMMatrixTranspose(
			m_parentEntity.GetTransformMatrix() *
			DirectX::XMMatrixPerspectiveFovRH(DirectX::XM_PIDIV4, kRenderRatio, 0.1f, 100.0f)
		)
	};

	D3D11_MAPPED_SUBRESOURCE mappedSubResource;
	pDeviceContext->Map(m_pConstantBuffer.Get(), 0u,
		D3D11_MAP_WRITE_DISCARD, 0u, &mappedSubResource);
	memcpy(mappedSubResource.pData, &cBuffer, sizeof(cBuffer));
	pDeviceContext->Unmap(m_pConstantBuffer.Get(), 0u);
}

/* ================================================== */

/*ConstantBufferPixel::ConstantBufferPixel(const Microsoft::WRL::ComPtr<ID3D11Device>& pDevice) {

	D3D11_BUFFER_DESC constantBufferDesc = {};
	constantBufferDesc.ByteWidth = sizeof(constantbuffer); // return total array size in bytes
	constantBufferDesc.Usage = D3D11_USAGE_DYNAMIC;	// to be able to update every frame
	constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constantBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; // cpu need permission to update buffer
	constantBufferDesc.MiscFlags = 0u;
	constantBufferDesc.StructureByteStride = 0u; // only single element
	D3D11_SUBRESOURCE_DATA constantSubResourceData = {};
	constantSubResourceData.pSysMem = &constantbuffer;
	DX::ThrowIfFailed(pDevice->CreateBuffer(&constantBufferDesc, &constantSubResourceData, &m_pConstantBuffer));
}

void ConstantBufferPixel::Bind(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContext) const {
	pDeviceContext->PSSetConstantBuffers(0u, 1u, m_pConstantBuffer.GetAddressOf());
}

void ConstantBufferPixel::Update(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContext, const CBuffer& cBuffer) {
	D3D11_MAPPED_SUBRESOURCE mappedSubResource;

	pDeviceContext->Map(m_pConstantBuffer.Get(), 0u,
		D3D11_MAP_WRITE_DISCARD, 0u, &mappedSubResource);

	memcpy(mappedSubResource.pData, &cBuffer, sizeof(cBuffer));

	pDeviceContext->Unmap(m_pConstantBuffer.Get(), 0u);
}*/