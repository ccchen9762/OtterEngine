#pragma once

#include "GraphicsResource.h"

class IndexBuffer : public GraphicsResource
{
public:
	IndexBuffer(const Graphics& graphics, const std::vector<unsigned short>& indices);
	~IndexBuffer() = default;

	void Bind(const Graphics& graphics) const override;

private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_pIndexBuffer;
};