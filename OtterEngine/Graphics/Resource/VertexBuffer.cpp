#include "VertexBuffer.h"

VertexBuffer::VertexBuffer(const Graphics& graphics, const void* verticesData, unsigned int stride, size_t size, bool isTriangle) :
	m_vertexBufferStride(stride), m_vertexBufferoffset(0u), m_isTriangle(isTriangle) {
	
	D3D11_BUFFER_DESC vertexBufferDesc = {};
	vertexBufferDesc.ByteWidth = stride * size; // return total array size in bytes
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0u;
	vertexBufferDesc.MiscFlags = 0u;
	vertexBufferDesc.StructureByteStride = m_vertexBufferStride;
	D3D11_SUBRESOURCE_DATA vertexSubResourceData = {};
	vertexSubResourceData.pSysMem = verticesData;
	
	DX::ThrowIfFailed(GetDevice(graphics)->CreateBuffer(&vertexBufferDesc, &vertexSubResourceData, &m_pVertexBuffer));
}

void VertexBuffer::Bind(const Graphics& graphics) const {
	
	GetDeviceContext(graphics)->IASetVertexBuffers(0u, 1u,
		m_pVertexBuffer.GetAddressOf(), &m_vertexBufferStride, &m_vertexBufferoffset);
	if(m_isTriangle)
		GetDeviceContext(graphics)->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	else
		GetDeviceContext(graphics)->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
}
