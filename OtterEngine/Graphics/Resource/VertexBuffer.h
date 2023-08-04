#pragma once

#include "GraphicsResource.h"

#include <DirectXMath.h>

struct Vertex {
	DirectX::XMVECTOR m_position;
	DirectX::XMFLOAT4 m_color;
};

class VertexBuffer : public GraphicsResource
{
public:
	VertexBuffer(const Microsoft::WRL::ComPtr<ID3D11Device>& pDevice, const std::vector<Vertex>& vertices);
	~VertexBuffer() = default;

	void Bind(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContext) const override;

private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_pVertexBuffer;
	unsigned int m_vertexBufferStride;
	unsigned int m_vertexBufferoffset;
};