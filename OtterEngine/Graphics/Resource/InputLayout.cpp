#include "InputLayout.h"

// AlignedByteOffset can use D3D11_APPEND_ALIGNED_ELEMENT to automatically assign
const D3D11_INPUT_ELEMENT_DESC InputLayout::s_inputElementDesc[] = {
	{ "SV_Position", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0,   D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR",		 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16u, D3D11_INPUT_PER_VERTEX_DATA, 0 }, // semantic should be without number, e.g. no COLOR0
};

const D3D11_INPUT_ELEMENT_DESC InputLayout::s_inputElementDescTextured[] = {
	{ "SV_Position", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0,   D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",	 0, DXGI_FORMAT_R32G32_FLOAT,		0, 16u, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

const D3D11_INPUT_ELEMENT_DESC InputLayout::s_inputElementDescShading[] = {
	{ "SV_Position", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0,   D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR",		 0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, 16u, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "NORMAL",		 0, DXGI_FORMAT_R32G32B32_FLOAT,	0, 32u, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

const D3D11_INPUT_ELEMENT_DESC InputLayout::s_inputElementDescTexturedShading[] = {
	{ "SV_Position", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0,   D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",	 0, DXGI_FORMAT_R32G32_FLOAT,		0, 16u, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "NORMAL",		 0, DXGI_FORMAT_R32G32B32_FLOAT,	0, 24u, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};


InputLayout::InputLayout(const Graphics& graphics, const std::vector<uint8_t>& vertexShaderBlob, LayoutType type) :
	GraphicsResource(GenerateUID(vertexShaderBlob, type)) {

	switch (type)
	{
	case InputLayout::LayoutType::Basic: {
		DX::ThrowIfFailed(GetDevice(graphics)->CreateInputLayout(s_inputElementDesc, sizeof(s_inputElementDesc) / sizeof(D3D11_INPUT_ELEMENT_DESC),
			vertexShaderBlob.data(), vertexShaderBlob.size(), &m_pInputLayout));
		break;
	}
	case InputLayout::LayoutType::Texture: {
		DX::ThrowIfFailed(GetDevice(graphics)->CreateInputLayout(s_inputElementDescTextured, sizeof(s_inputElementDescTextured) / sizeof(D3D11_INPUT_ELEMENT_DESC),
			vertexShaderBlob.data(), vertexShaderBlob.size(), &m_pInputLayout));
		break;
	}
	case InputLayout::LayoutType::Shading: {
		DX::ThrowIfFailed(GetDevice(graphics)->CreateInputLayout(s_inputElementDescShading, sizeof(s_inputElementDescShading) / sizeof(D3D11_INPUT_ELEMENT_DESC),
			vertexShaderBlob.data(), vertexShaderBlob.size(), &m_pInputLayout));
		break;
	}
	case InputLayout::LayoutType::TextureShading: {
		DX::ThrowIfFailed(GetDevice(graphics)->CreateInputLayout(s_inputElementDescTexturedShading, sizeof(s_inputElementDescTexturedShading) / sizeof(D3D11_INPUT_ELEMENT_DESC),
			vertexShaderBlob.data(), vertexShaderBlob.size(), &m_pInputLayout));
		break;
	}
	}
}

void InputLayout::Bind(const Graphics& graphics) const {
	GetDeviceContext(graphics)->IASetInputLayout(m_pInputLayout.Get());
}
