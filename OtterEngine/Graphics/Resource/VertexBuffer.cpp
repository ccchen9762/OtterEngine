#include "VertexBuffer.h"

VertexBuffer::VertexBuffer(ID3D11Device* pDevice, const std::vector<Vertex>& vertices) :
	m_vertexBufferStride(sizeof(Vertex)), m_vertexBufferoffset(0u) {

	// triangle
	/*const Vertex vertices[] = {
		{DirectX::XMVectorSet(0.0f,  0.5f,  0.0f, 1.0f), DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f)},
		{DirectX::XMVectorSet(0.5f, -0.5f,  0.0f, 1.0f), DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f)},
		{DirectX::XMVectorSet(-0.5f, -0.5f,  0.0f, 1.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f)},
	};*/

	// cube
	/*
	const Vertex vertices[] = {
		{DirectX::XMVectorSet( -1.0f, -1.0f, -1.0f, 1.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f)},
		{DirectX::XMVectorSet(  1.0f, -1.0f, -1.0f, 1.0f), DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f)},
		{DirectX::XMVectorSet(  1.0f,  1.0f, -1.0f, 1.0f), DirectX::XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f)},
		{DirectX::XMVectorSet( -1.0f,  1.0f, -1.0f, 1.0f), DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f)},
		{DirectX::XMVectorSet( -1.0f, -1.0f,  1.0f, 1.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f)},
		{DirectX::XMVectorSet(  1.0f, -1.0f,  1.0f, 1.0f), DirectX::XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f)},
		{DirectX::XMVectorSet(  1.0f,  1.0f,  1.0f, 1.0f), DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f)},
		{DirectX::XMVectorSet( -1.0f,  1.0f,  1.0f, 1.0f), DirectX::XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f)},
	};*/
	
	D3D11_BUFFER_DESC vertexBufferDesc = {};
	vertexBufferDesc.ByteWidth = sizeof(Vertex) * vertices.size(); // return total array size in bytes
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0u;
	vertexBufferDesc.MiscFlags = 0u;
	vertexBufferDesc.StructureByteStride = m_vertexBufferStride;
	D3D11_SUBRESOURCE_DATA vertexSubResourceData = {};
	vertexSubResourceData.pSysMem = vertices.data();
	
	DX::ThrowIfFailed(pDevice->CreateBuffer(&vertexBufferDesc, &vertexSubResourceData, &m_pVertexBuffer));
}

void VertexBuffer::Bind(ID3D11DeviceContext* pDeviceContext) const {
	
	pDeviceContext->IASetVertexBuffers(0u, 1u, 
		m_pVertexBuffer.GetAddressOf(), &m_vertexBufferStride, &m_vertexBufferoffset);
	pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}
