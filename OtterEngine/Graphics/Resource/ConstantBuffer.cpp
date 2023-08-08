#include "ConstantBuffer.h"

#include "OtterEngine/Common/constants.h"

ConstantBufferTransformation::ConstantBufferTransformation (const Graphics& graphics, const Entity& parentEntity)
	: m_parentEntity(parentEntity) {

	// in Left Hand System (LH) Z axis direction is away from screen, RH toward screen
	// remember XMMatrixTranspose only need once
	const CBuffer cBuffer = {
		DirectX::XMMatrixTranspose(
			m_parentEntity.GetTransformMatrix() *
			m_parentEntity.GetViewProjectionMatrix()
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
	DX::ThrowIfFailed(GetDevice(graphics)->CreateBuffer(&constantBufferDesc, &constantSubResourceData, &m_pConstantBuffer));
}

void ConstantBufferTransformation::Bind(const Graphics& graphics) const {
	Update(graphics);
	GetDeviceContext(graphics)->VSSetConstantBuffers(0u, 1u, m_pConstantBuffer.GetAddressOf());
}

void ConstantBufferTransformation::Update(const Graphics& graphics) const {

	const CBuffer cBuffer = {
		DirectX::XMMatrixTranspose(
			m_parentEntity.GetTransformMatrix() *
			m_parentEntity.GetViewProjectionMatrix()
		)
	};

	D3D11_MAPPED_SUBRESOURCE mappedSubResource;
	DX::ThrowIfFailed(GetDeviceContext(graphics)->Map(m_pConstantBuffer.Get(), 0u,
		D3D11_MAP_WRITE_DISCARD, 0u, &mappedSubResource));
	memcpy(mappedSubResource.pData, &cBuffer, sizeof(cBuffer));
	GetDeviceContext(graphics)->Unmap(m_pConstantBuffer.Get(), 0u);
}

/* ================================================== */

ConstantBufferVertex::ConstantBufferVertex(const Graphics& graphics) {

	D3D11_BUFFER_DESC constantBufferDesc = {};
	//constantBufferDesc.ByteWidth = sizeof(cBuffer); // return total array size in bytes
	constantBufferDesc.Usage = D3D11_USAGE_DYNAMIC;	// to be able to update every frame
	constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constantBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; // cpu need permission to update buffer
	constantBufferDesc.MiscFlags = 0u;
	constantBufferDesc.StructureByteStride = 0u; // only single element
	D3D11_SUBRESOURCE_DATA constantSubResourceData = {};
	//constantSubResourceData.pSysMem = &cBuffer;
	DX::ThrowIfFailed(GetDevice(graphics)->CreateBuffer(&constantBufferDesc, &constantSubResourceData, &m_pConstantBuffer));
}

void ConstantBufferVertex::Bind(const Graphics& graphics) const {
	Update(graphics);
	GetDeviceContext(graphics)->VSSetConstantBuffers(0u, 1u, m_pConstantBuffer.GetAddressOf());
}

void ConstantBufferVertex::Update(const Graphics& graphics) const {

	D3D11_MAPPED_SUBRESOURCE mappedSubResource;
	DX::ThrowIfFailed(GetDeviceContext(graphics)->Map(m_pConstantBuffer.Get(), 0u,
		D3D11_MAP_WRITE_DISCARD, 0u, &mappedSubResource));
	//memcpy(mappedSubResource.pData, &cBuffer, sizeof(cBuffer));
	GetDeviceContext(graphics)->Unmap(m_pConstantBuffer.Get(), 0u);
}

/* ================================================== */

ConstantBufferPixel::ConstantBufferPixel(const Graphics& graphics) {

	D3D11_BUFFER_DESC constantBufferDesc = {};
	//constantBufferDesc.ByteWidth = sizeof(cBuffer); // return total array size in bytes
	constantBufferDesc.Usage = D3D11_USAGE_DYNAMIC;	// to be able to update every frame
	constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constantBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; // cpu need permission to update buffer
	constantBufferDesc.MiscFlags = 0u;
	constantBufferDesc.StructureByteStride = 0u; // only single element
	D3D11_SUBRESOURCE_DATA constantSubResourceData = {};
	//constantSubResourceData.pSysMem = &cBuffer;
	DX::ThrowIfFailed(GetDevice(graphics)->CreateBuffer(&constantBufferDesc, &constantSubResourceData, &m_pConstantBuffer));
}

void ConstantBufferPixel::Bind(const Graphics& graphics) const {
	Update(graphics);
	GetDeviceContext(graphics)->PSSetConstantBuffers(0u, 1u, m_pConstantBuffer.GetAddressOf());
}

void ConstantBufferPixel::Update(const Graphics& graphics) const {

	D3D11_MAPPED_SUBRESOURCE mappedSubResource;
	DX::ThrowIfFailed(GetDeviceContext(graphics)->Map(m_pConstantBuffer.Get(), 0u,
		D3D11_MAP_WRITE_DISCARD, 0u, &mappedSubResource));
	//memcpy(mappedSubResource.pData, &cBuffer, sizeof(cBuffer));
	GetDeviceContext(graphics)->Unmap(m_pConstantBuffer.Get(), 0u);
}