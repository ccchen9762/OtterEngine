#pragma once

#include "GraphicsResource.h"

#include <DirectXMath.h>

#include "OtterEngine/Entity/Entity.h"
#include "OtterEngine/Camera/Camera.h"

class ConstantBufferTransformation : public GraphicsResource
{
private:
	struct CBuffer {
		DirectX::XMMATRIX model;
		DirectX::XMMATRIX modelViewProjection;
	};

public:
	ConstantBufferTransformation(const Graphics& graphics, const Entity& parentEntity);
	~ConstantBufferTransformation() = default;

	virtual void Bind(const Graphics& graphics) const override;

private:
	virtual void Update(const Graphics& graphics) const;

private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_pConstantBuffer;
	const Entity& m_parentEntity;
};

// ========================================

template <typename T>
class ConstantBufferVertex : public GraphicsResource
{
public:
	ConstantBufferVertex(const Graphics& graphics, const T& t) {
		D3D11_BUFFER_DESC constantBufferDesc = {};
		constantBufferDesc.ByteWidth = sizeof(T); // return total array size in bytes
		constantBufferDesc.Usage = D3D11_USAGE_DYNAMIC;	// to be able to update every frame
		constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		constantBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; // cpu need permission to update buffer
		constantBufferDesc.MiscFlags = 0u;
		constantBufferDesc.StructureByteStride = 0u; // only single element
		D3D11_SUBRESOURCE_DATA constantSubResourceData = {};
		constantSubResourceData.pSysMem = &t;
		DX::ThrowIfFailed(GetDevice(graphics)->CreateBuffer(&constantBufferDesc, &constantSubResourceData, &m_pConstantBuffer));
	}
	~ConstantBufferVertex() = default;

	
	void Bind(const Graphics& graphics) const override {
		GetDeviceContext(graphics)->VSSetConstantBuffers(0u, 1u, m_pConstantBuffer.GetAddressOf());
	}

	void Update(const Graphics& graphics, const T& t) const {
		D3D11_MAPPED_SUBRESOURCE mappedSubResource;
		DX::ThrowIfFailed(GetDeviceContext(graphics)->Map(m_pConstantBuffer.Get(), 0u,
			D3D11_MAP_WRITE_DISCARD, 0u, &mappedSubResource));
		memcpy(mappedSubResource.pData, &t, sizeof(T));
		GetDeviceContext(graphics)->Unmap(m_pConstantBuffer.Get(), 0u);

		Bind(graphics);
	}

private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_pConstantBuffer;
};

// ========================================

template <typename T>
class ConstantBufferPixel : public GraphicsResource
{
public:
	ConstantBufferPixel(const Graphics& graphics, const T& t) {
		D3D11_BUFFER_DESC constantBufferDesc = {};
		constantBufferDesc.ByteWidth = sizeof(T); // return total array size in bytes
		constantBufferDesc.Usage = D3D11_USAGE_DYNAMIC;	// to be able to update every frame
		constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		constantBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; // cpu need permission to update buffer
		constantBufferDesc.MiscFlags = 0u;
		constantBufferDesc.StructureByteStride = 0u; // only single element
		D3D11_SUBRESOURCE_DATA constantSubResourceData = {};
		constantSubResourceData.pSysMem = &t;
		DX::ThrowIfFailed(GetDevice(graphics)->CreateBuffer(&constantBufferDesc, &constantSubResourceData, &m_pConstantBuffer));
	}
	~ConstantBufferPixel() = default;


	void Bind(const Graphics& graphics) const override {
		GetDeviceContext(graphics)->PSSetConstantBuffers(0u, 1u, m_pConstantBuffer.GetAddressOf());
	}

	void Update(const Graphics& graphics, const T& t) const {
		D3D11_MAPPED_SUBRESOURCE mappedSubResource;
		DX::ThrowIfFailed(GetDeviceContext(graphics)->Map(m_pConstantBuffer.Get(), 0u,
			D3D11_MAP_WRITE_DISCARD, 0u, &mappedSubResource));
		memcpy(mappedSubResource.pData, &t, sizeof(T));
		GetDeviceContext(graphics)->Unmap(m_pConstantBuffer.Get(), 0u);

		Bind(graphics);
	}

private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_pConstantBuffer;
};