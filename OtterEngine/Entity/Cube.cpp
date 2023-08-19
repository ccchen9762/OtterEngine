#include "Cube.h"

#include "OtterEngine/Graphics/Resource/VertexShader.h"
#include "OtterEngine/Graphics/Resource/PixelShader.h"
#include "OtterEngine/Graphics/Resource/InputLayout.h"
#include "OtterEngine/Graphics/Resource/VertexBuffer.h"
#include "OtterEngine/Graphics/Resource/IndexBuffer.h"
#include "OtterEngine/Graphics/Resource/ConstantBuffer.h"

std::vector<Vertex> Cube::s_vertices;
std::vector<unsigned short> Cube::s_indices;

std::vector<std::unique_ptr<GraphicsResource>> Cube::s_commonResources;

Cube::Cube(const Graphics& graphics, const Vector3& translation, const Vector3& rotation, const Vector3& scale,
	const DirectX::XMMATRIX& viewProjectionMatrix, bool isStatic)
	: Entity(translation, rotation, scale, s_indices.size(), viewProjectionMatrix, isStatic) {

	if (s_commonResources.empty()) {
		GenerateMesh(); // generate static vertices and indices
		
		// shaders & layout
		if (kRenderMethod == 0) {
			std::unique_ptr<VertexShader> pVertexShader = std::make_unique<VertexShader>(graphics, L"GouraudVertexShader.cso");
			const std::vector<uint8_t> vertexShaderBlob = pVertexShader->GetVertexShaderBlob();
			s_commonResources.push_back(std::move(pVertexShader));
			s_commonResources.push_back(std::make_unique<PixelShader>(graphics, L"GouraudPixelShader.cso"));
			s_commonResources.push_back(std::make_unique<InputLayout>(graphics, vertexShaderBlob, InputLayout::LayoutType::Shading));
		}
		else if (kRenderMethod == 1) {
			std::unique_ptr<VertexShader> pVertexShader = std::make_unique<VertexShader>(graphics, L"PhongVertexShader.cso");
			const std::vector<uint8_t> vertexShaderBlob = pVertexShader->GetVertexShaderBlob();
			s_commonResources.push_back(std::move(pVertexShader));
			s_commonResources.push_back(std::make_unique<PixelShader>(graphics, L"PhongPixelShader.cso"));
			s_commonResources.push_back(std::make_unique<InputLayout>(graphics, vertexShaderBlob, InputLayout::LayoutType::Shading));
		}
		
		// buffers
		s_commonResources.push_back(std::make_unique<VertexBuffer>(graphics, 
			s_vertices.data(), static_cast<unsigned int>(sizeof(Vertex)), s_vertices.size()));
		s_commonResources.push_back(std::make_unique<IndexBuffer>(graphics, s_indices));
	}
	
	m_uniqueResources.push_back(std::make_unique<ConstantBufferTransformation>(graphics, *this));
}

void Cube::GenerateMesh() {
	const DirectX::XMVECTOR verticesPosition[8] = {
		{DirectX::XMVectorSet(-0.5f, -0.5f, -0.5f,  1.0f)},
		{DirectX::XMVectorSet( 0.5f, -0.5f, -0.5f,  1.0f)},
		{DirectX::XMVectorSet( 0.5f,  0.5f, -0.5f,  1.0f)},
		{DirectX::XMVectorSet(-0.5f,  0.5f, -0.5f,  1.0f)},
		{DirectX::XMVectorSet(-0.5f, -0.5f,  0.5f,  1.0f)},
		{DirectX::XMVectorSet( 0.5f, -0.5f,  0.5f,  1.0f)},
		{DirectX::XMVectorSet( 0.5f,  0.5f,  0.5f,  1.0f)},
		{DirectX::XMVectorSet(-0.5f,  0.5f,  0.5f,  1.0f)},
	};

	const unsigned short indices[24] = {
		0, 1, 2, 3, // back
		6, 5, 4, 7, // front
		7, 4, 0, 3, // left
		2, 1, 5, 6, // right
		4, 5, 1, 0, // bottom
		2, 6, 7, 3, // top
	};

	const Color4 verticesColor[6] = {
		{1.0f, 1.0f, 0.0f, 1.0f},
		{1.0f, 0.0f, 1.0f, 1.0f},
		{0.0f, 1.0f, 1.0f, 1.0f},
		{1.0f, 0.0f, 0.0f, 1.0f},
		{0.0f, 1.0f, 0.0f, 1.0f},
		{0.0f, 0.0f, 1.0f, 1.0f},
	};

	const Normal verticesNormal[6] = {
		{ 0.0f,  0.0f, -1.0f},
		{ 0.0f,  0.0f,  1.0f},
		{-1.0f,  0.0f,  0.0f},
		{ 1.0f,  0.0f,  0.0f},
		{ 0.0f, -1.0f,  0.0f},
		{ 0.0f,  1.0f,  0.0f},
	};

	s_vertices.resize(24);
	for (int i = 0; i < 24; i++) {
		s_vertices[i].m_position = verticesPosition[indices[i]];
		s_vertices[i].m_color = verticesColor[i/4];
		s_vertices[i].m_normal = verticesNormal[i/4];
	}

	s_indices = {
		 0,  1,  2,   2,  3,  0, // back
		 4,  5,  6,   6,  7,  4, // front
		 8,  9, 10,  10, 11,  8, // left
		12, 13, 14,  14, 15, 12, // right
		20, 21, 22,  22, 23, 20, // bottom
		16, 17, 18,  18, 19, 16, // top
	};
}

const std::vector<std::unique_ptr<GraphicsResource>>& Cube::GetCommonResources() const {
	return s_commonResources;
}
