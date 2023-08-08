#pragma once

#include "GraphicsResource.h"

#include <DirectXMath.h>

#include "OtterEngine/Common/Vertex.h"

class VertexBuffer : public GraphicsResource
{
public:
	VertexBuffer(const Graphics& graphics, const void* verticesData, unsigned int stride, size_t size, bool isTriangle = true);
	~VertexBuffer() = default;

	void Bind(const Graphics& graphics) const override;

private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_pVertexBuffer;
	unsigned int m_vertexBufferStride;
	unsigned int m_vertexBufferoffset;
	bool m_isTriangle;
};