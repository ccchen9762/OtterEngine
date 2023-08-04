#include "Triangle.h"

const std::vector<Vertex> Triangle::s_vertices = {
	{DirectX::XMVectorSet(0.0f,  0.5f,  0.0f, 1.0f), DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f)},
	{DirectX::XMVectorSet(0.5f, -0.5f,  0.0f, 1.0f), DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f)},
	{DirectX::XMVectorSet(-0.5f, -0.5f,  0.0f, 1.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f)},
};

const std::vector<unsigned short> Triangle::s_indices = {
	0,1,2
};

Triangle::Triangle(const Microsoft::WRL::ComPtr<ID3D11Device>& pDevice, Vector3 rotation, Vector3 translation, Vector3 revolution, Vector3 scale)
	: Entity(rotation, translation, revolution, scale, s_indices.size()) {

	// shaders & layout
	std::unique_ptr<VertexShader> pVertexShader = std::make_unique<VertexShader>(pDevice.Get());
	const std::vector<uint8_t> vertexShaderBlob = pVertexShader->GetVertexShaderBlob();
	m_graphicsResources.push_back(std::move(pVertexShader));
	m_graphicsResources.push_back(std::make_unique<PixelShader>(pDevice.Get()));
	m_graphicsResources.push_back(std::make_unique<InputLayout>(pDevice.Get(), vertexShaderBlob));

	// buffers
	m_graphicsBuffers.push_back(std::make_unique<VertexBuffer>(pDevice.Get(), s_vertices));
	m_graphicsBuffers.push_back(std::make_unique<IndexBuffer>(pDevice.Get(), s_indices));
	m_graphicsBuffers.push_back(std::make_unique<ConstantBuffer>(pDevice.Get(), GetTransformMatrix()));
}