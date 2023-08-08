#include "Cube.h"

#include "OtterEngine/Graphics/Resource/VertexShader.h"
#include "OtterEngine/Graphics/Resource/PixelShader.h"
#include "OtterEngine/Graphics/Resource/InputLayout.h"
#include "OtterEngine/Graphics/Resource/VertexBuffer.h"
#include "OtterEngine/Graphics/Resource/IndexBuffer.h"
#include "OtterEngine/Graphics/Resource/ConstantBuffer.h"

const std::vector<Vertex> Cube::s_vertices = {
	{DirectX::XMVectorSet(-0.5f, -0.5f, -0.5f, 1.0f), {0.0f, 0.0f, 0.0f, 1.0f}},
	{DirectX::XMVectorSet( 0.5f, -0.5f, -0.5f, 1.0f), {1.0f, 0.0f, 0.0f, 1.0f}},
	{DirectX::XMVectorSet( 0.5f,  0.5f, -0.5f, 1.0f), {1.0f, 1.0f, 0.0f, 1.0f}},
	{DirectX::XMVectorSet(-0.5f,  0.5f, -0.5f, 1.0f), {0.0f, 1.0f, 0.0f, 1.0f}},
	{DirectX::XMVectorSet(-0.5f, -0.5f,  0.5f, 1.0f), {0.0f, 0.0f, 1.0f, 1.0f}},
	{DirectX::XMVectorSet( 0.5f, -0.5f,  0.5f, 1.0f), {1.0f, 0.0f, 1.0f, 1.0f}},
	{DirectX::XMVectorSet( 0.5f,  0.5f,  0.5f, 1.0f), {1.0f, 1.0f, 1.0f, 1.0f}},
	{DirectX::XMVectorSet(-0.5f,  0.5f,  0.5f, 1.0f), {0.0f, 1.0f, 1.0f, 1.0f}},
};

const std::vector<unsigned short> Cube::s_indices = {
	0, 1, 2,  2, 3, 0, // back
	6, 5, 4,  4, 7, 6, // front
	7, 4, 0,  0, 3, 7, // left
	2, 1, 5,  5, 6, 2, // right
	2, 6, 7,  7, 3, 2, // top
	4, 5, 1,  1, 0, 4, // bottom
};

std::vector<std::unique_ptr<GraphicsResource>> Cube::s_commonResources;

Cube::Cube(const Graphics& graphics, Vector3 rotation, Vector3 translation, Vector3 revolution, Vector3 scale, 
	const DirectX::XMMATRIX& viewProjectionMatrix, float speed)
	: Entity(rotation, translation, revolution, scale, s_indices.size(), viewProjectionMatrix, speed) {

	if (s_commonResources.empty()) {
		// shaders & layout
		std::unique_ptr<VertexShader> pVertexShader = std::make_unique<VertexShader>(graphics, L"VertexShader.cso");
		const std::vector<uint8_t> vertexShaderBlob = pVertexShader->GetVertexShaderBlob();
		s_commonResources.push_back(std::move(pVertexShader));
		s_commonResources.push_back(std::make_unique<PixelShader>(graphics, L"PixelShader.cso"));
		s_commonResources.push_back(std::make_unique<InputLayout>(graphics, vertexShaderBlob));

		// buffers
		s_commonResources.push_back(std::make_unique<VertexBuffer>(graphics, 
			s_vertices.data(), sizeof(Vertex), s_vertices.size()));
		s_commonResources.push_back(std::make_unique<IndexBuffer>(graphics, s_indices));
	}
	
	m_uniqueResources.push_back(std::make_unique<ConstantBufferTransformation>(graphics, *this));
}

const std::vector<std::unique_ptr<GraphicsResource>>& Cube::GetCommonResources() const {
	return s_commonResources;
}
