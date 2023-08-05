#include "InputLayout.h"

InputLayout::InputLayout(const Graphics& graphics, const std::vector<uint8_t>& vertexShaderBlob) {
	
	// AlignedByteOffset can use D3D11_APPEND_ALIGNED_ELEMENT to automatically assign
	const D3D11_INPUT_ELEMENT_DESC s_inputElementDesc[] = {
		{ "SV_Position", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0,   D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",		 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16u, D3D11_INPUT_PER_VERTEX_DATA, 0 }, // semantic takes COLOR instead of COLOR0 ?
	};
	
	DX::ThrowIfFailed(GetDevice(graphics)->CreateInputLayout(s_inputElementDesc, sizeof(s_inputElementDesc) / sizeof(D3D11_INPUT_ELEMENT_DESC),
		vertexShaderBlob.data(), vertexShaderBlob.size(), &m_pInputLayout));
}

void InputLayout::Bind(const Graphics& graphics) const {
	GetDeviceContext(graphics)->IASetInputLayout(m_pInputLayout.Get());
}
