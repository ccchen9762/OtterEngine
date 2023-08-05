#pragma once

#include "GraphicsResource.h"

#include <DirectXMath.h>

#include "OtterEngine/Entity/Entity.h"

struct CBuffer {
	DirectX::XMMATRIX transform;
};

class ConstantBufferVertex : public GraphicsResource
{
public:
	ConstantBufferVertex(const Microsoft::WRL::ComPtr<ID3D11Device>& pDevice, const Entity& parentEntity);
	~ConstantBufferVertex() = default;

	void Bind(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContext) const override;

private:
	void Update(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContext) const;

private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_pConstantBuffer;
	const Entity& m_parentEntity;
};

/*class ConstantBufferPixel : public GraphicsResource
{
public:
	ConstantBufferPixel(const Microsoft::WRL::ComPtr<ID3D11Device>& pDevice);
	~ConstantBufferPixel() = default;

	void Bind(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContext) const override;

	void Update(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContext, const CBuffer& cBuffer);

private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_pConstantBuffer;
};*/