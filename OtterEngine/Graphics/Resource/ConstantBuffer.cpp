#include "ConstantBuffer.h"

#include "OtterEngine/Common/constants.h"

ConstantBuffer::ConstantBuffer(const Microsoft::WRL::ComPtr<ID3D11Device>& pDevice, const DirectX::XMMATRIX& transformation) {
	// in Left Hand System (LH) Z axis direction is away from screen, RH toward screen
	// remember XMMatrixTranspose only need once
	const cbuf constantbuffer = {
		DirectX::XMMatrixTranspose(
			transformation *
			DirectX::XMMatrixPerspectiveRH(1.0, kRenderRatio, 0.1f, 20.0f)
		)
	};

	D3D11_BUFFER_DESC constantBufferDesc = {};
	constantBufferDesc.ByteWidth = sizeof(constantbuffer); // return total array size in bytes
	constantBufferDesc.Usage = D3D11_USAGE_DYNAMIC;	// to be able to update every frame
	constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constantBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; // cpu need permission to update buffer
	constantBufferDesc.MiscFlags = 0u;
	constantBufferDesc.StructureByteStride = 0u; // only single element
	D3D11_SUBRESOURCE_DATA constantSubResourceData = {};
	constantSubResourceData.pSysMem = &constantbuffer;
	DX::ThrowIfFailed(pDevice->CreateBuffer(&constantBufferDesc, &constantSubResourceData, &m_pConstantBuffer));
}

void ConstantBuffer::Bind(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContext) const {
	pDeviceContext->VSSetConstantBuffers(0u, 1u, m_pConstantBuffer.GetAddressOf());
}
