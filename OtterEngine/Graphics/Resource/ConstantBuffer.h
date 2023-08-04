#pragma once

#include "GraphicsResource.h"

#include <DirectXMath.h>

struct cbuf {
	DirectX::XMMATRIX transform;
};

class ConstantBuffer : public GraphicsResource
{
public:
	ConstantBuffer(const Microsoft::WRL::ComPtr<ID3D11Device>& pDevice, const DirectX::XMMATRIX& transformation);
	~ConstantBuffer() = default;

	void Bind(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContext) const override;

private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_pConstantBuffer;
};