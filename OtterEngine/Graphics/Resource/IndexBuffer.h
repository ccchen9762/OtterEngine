#pragma once

#include "GraphicsResource.h"

class IndexBuffer : public GraphicsResource
{
public:
	IndexBuffer(ID3D11Device* pDevice, const std::vector<unsigned short>& indices);
	~IndexBuffer() = default;

	void Bind(ID3D11DeviceContext* pDeviceContext) const;

private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_pIndexBuffer;
};