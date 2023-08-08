#include "IndexBuffer.h"

IndexBuffer::IndexBuffer(const Graphics& graphics, const std::vector<unsigned short>& indices) {

	D3D11_BUFFER_DESC indexBufferDesc = {};
	indexBufferDesc.ByteWidth = static_cast<UINT>(sizeof(unsigned short) * indices.size()); // return total array size in bytes
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0u;
	indexBufferDesc.MiscFlags = 0u;
	indexBufferDesc.StructureByteStride = sizeof(unsigned short);
	D3D11_SUBRESOURCE_DATA indexSubResourceData = {};
	indexSubResourceData.pSysMem = indices.data();
	
	DX::ThrowIfFailed(GetDevice(graphics)->CreateBuffer(&indexBufferDesc, &indexSubResourceData, &m_pIndexBuffer));
}

void IndexBuffer::Bind(const Graphics& graphics) const {
	GetDeviceContext(graphics)->IASetIndexBuffer(m_pIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0u);
}
