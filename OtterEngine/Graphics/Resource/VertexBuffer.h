#pragma once

#include "GraphicsResource.h"

class VertexBuffer : public GraphicsResource
{
public:
	enum class Topology {
		Line = 0, Triangle = 1
	};

public:
	VertexBuffer(const Graphics& graphics, const void* verticesData, 
		unsigned int stride, size_t size, Topology topology, const std::wstring& tag);
	~VertexBuffer() = default;
	
	static std::wstring GenerateUID(const void* verticesData, unsigned int stride, size_t size, Topology topology, const std::wstring& tag) {
		return L"VertexBuffer#" + std::to_wstring(static_cast<int>(topology)) + L"#" + tag; 
	}

	void Bind(const Graphics& graphics) const override;

private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_pVertexBuffer;
	unsigned int m_stride;
	unsigned int m_offset;
	Topology m_topology;
};