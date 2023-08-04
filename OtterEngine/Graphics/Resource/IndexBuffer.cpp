#include "IndexBuffer.h"

IndexBuffer::IndexBuffer(const Microsoft::WRL::ComPtr<ID3D11Device>& pDevice, const std::vector<unsigned short>& indices) {
	
	// indices are 2 bytes by default
	/*unsigned short indices[] = {
		0,1,2
	};*/
	/*
	const unsigned short indices[] = {
		0, 1, 2,  2, 3, 0, // back
		6, 5, 4,  4, 7, 6, // front
		7, 4, 0,  0, 3, 7, // left
		2, 1, 5,  5, 6, 2, // right
		2, 6, 7,  7, 3, 2, // top
		4, 5, 1,  1, 0, 4, // bottom
	};*/


	D3D11_BUFFER_DESC indexBufferDesc = {};
	indexBufferDesc.ByteWidth = sizeof(unsigned short) * indices.size(); // return total array size in bytes
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0u;
	indexBufferDesc.MiscFlags = 0u;
	indexBufferDesc.StructureByteStride = sizeof(unsigned short);
	D3D11_SUBRESOURCE_DATA indexSubResourceData = {};
	indexSubResourceData.pSysMem = indices.data();
	
	DX::ThrowIfFailed(pDevice->CreateBuffer(&indexBufferDesc, &indexSubResourceData, &m_pIndexBuffer));
}

void IndexBuffer::Bind(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContext) const {
	pDeviceContext->IASetIndexBuffer(m_pIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0u);
}
