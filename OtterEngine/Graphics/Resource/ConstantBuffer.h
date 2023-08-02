#pragma once

#include "GraphicsResource.h"

#include <DirectXMath.h>

struct cbuf {
	DirectX::XMMATRIX transform;
};

class ConstantBuffer : public GraphicsResource
{
public:
	ConstantBuffer(ID3D11Device* pDevice, const DirectX::XMMATRIX& transformation);
	~ConstantBuffer() = default;

	void Bind(ID3D11DeviceContext* pDeviceContext) const;

private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_pConstantBuffer;
};