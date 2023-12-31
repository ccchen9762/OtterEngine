#include "VertexBuffer.h"

VertexBuffer::VertexBuffer(const Graphics& graphics, const void* verticesData, 
	unsigned int stride, size_t size, Topology topology, const std::wstring& tag) :
	GraphicsResource(GenerateUID(verticesData, stride, size, topology, tag)),
	m_stride(stride), m_offset(0u), m_topology(topology) {
	
	D3D11_BUFFER_DESC vertexBufferDesc = {};
	vertexBufferDesc.ByteWidth = static_cast<UINT>(stride * size); // return total array size in bytes
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0u;
	vertexBufferDesc.MiscFlags = 0u;
	vertexBufferDesc.StructureByteStride = m_stride;
	D3D11_SUBRESOURCE_DATA vertexSubResourceData = {};
	vertexSubResourceData.pSysMem = verticesData;
	
	DX::ThrowIfFailed(GetDevice(graphics)->CreateBuffer(&vertexBufferDesc, &vertexSubResourceData, &m_pVertexBuffer));
}

void VertexBuffer::Bind(const Graphics& graphics) const {
	
	GetDeviceContext(graphics)->IASetVertexBuffers(0u, 1u,
		m_pVertexBuffer.GetAddressOf(), &m_stride, &m_offset);

	switch (m_topology)
	{
	case VertexBuffer::Topology::Line:
		GetDeviceContext(graphics)->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
		break;
	case VertexBuffer::Topology::Triangle:
		GetDeviceContext(graphics)->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		break;
	}
}
