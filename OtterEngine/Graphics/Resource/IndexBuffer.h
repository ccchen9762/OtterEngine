#pragma once

#include "GraphicsResource.h"

class IndexBuffer : public GraphicsResource
{
public:
	IndexBuffer(const Microsoft::WRL::ComPtr<ID3D11Device>& pDevice, const std::vector<unsigned short>& indices);
	~IndexBuffer() = default;

	void Bind(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContext) const override;

private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_pIndexBuffer;
};