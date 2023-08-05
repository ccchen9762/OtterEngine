#include "Cube.h"

#include "OtterEngine/Graphics/Resource/VertexShader.h"
#include "OtterEngine/Graphics/Resource/PixelShader.h"
#include "OtterEngine/Graphics/Resource/InputLayout.h"
#include "OtterEngine/Graphics/Resource/VertexBuffer.h"
#include "OtterEngine/Graphics/Resource/IndexBuffer.h"
#include "OtterEngine/Graphics/Resource/ConstantBuffer.h"

const std::vector<Vertex> Cube::s_vertices = {
	{DirectX::XMVectorSet(-1.0f, -1.0f, -1.0f, 1.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f)},
	{DirectX::XMVectorSet( 1.0f, -1.0f, -1.0f, 1.0f), DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f)},
	{DirectX::XMVectorSet( 1.0f,  1.0f, -1.0f, 1.0f), DirectX::XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f)},
	{DirectX::XMVectorSet(-1.0f,  1.0f, -1.0f, 1.0f), DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f)},
	{DirectX::XMVectorSet(-1.0f, -1.0f,  1.0f, 1.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f)},
	{DirectX::XMVectorSet( 1.0f, -1.0f,  1.0f, 1.0f), DirectX::XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f)},
	{DirectX::XMVectorSet( 1.0f,  1.0f,  1.0f, 1.0f), DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f)},
	{DirectX::XMVectorSet(-1.0f,  1.0f,  1.0f, 1.0f), DirectX::XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f)},
};

const std::vector<unsigned short> Cube::s_indices = {
	0, 1, 2,  2, 3, 0, // back
	6, 5, 4,  4, 7, 6, // front
	7, 4, 0,  0, 3, 7, // left
	2, 1, 5,  5, 6, 2, // right
	2, 6, 7,  7, 3, 2, // top
	4, 5, 1,  1, 0, 4, // bottom
};

Cube::Cube(const Microsoft::WRL::ComPtr<ID3D11Device>& pDevice, Vector3 rotation, Vector3 translation, Vector3 revolution, Vector3 scale, float speed)
	: Entity(rotation, translation, revolution, scale, s_indices.size(), speed) {

	// shaders & layout
	std::unique_ptr<VertexShader> pVertexShader = std::make_unique<VertexShader>(pDevice.Get());
	const std::vector<uint8_t> vertexShaderBlob = pVertexShader->GetVertexShaderBlob();
	m_graphicsResources.push_back(std::move(pVertexShader));
	m_graphicsResources.push_back(std::make_unique<PixelShader>(pDevice.Get()));
	m_graphicsResources.push_back(std::make_unique<InputLayout>(pDevice.Get(), vertexShaderBlob));

	// buffers
	m_graphicsBuffers.push_back(std::make_unique<VertexBuffer>(pDevice.Get(), s_vertices));
	m_graphicsBuffers.push_back(std::make_unique<IndexBuffer>(pDevice.Get(), s_indices));
	m_graphicsBuffers.push_back(std::make_unique<ConstantBufferVertex>(pDevice.Get(), *this));
}
