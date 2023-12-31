#include "Cube.h"

#include "OtterEngine/Graphics/ResourcePool.h"
#include "OtterEngine/Graphics/Resource/VertexBuffer.h"
#include "OtterEngine/Graphics/Resource/IndexBuffer.h"
#include "OtterEngine/Graphics/Resource/DepthStencil.h"
#include "OtterEngine/Graphics/Resource/VertexShader.h"
#include "OtterEngine/Graphics/Resource/PixelShader.h"
#include "OtterEngine/Graphics/Resource/InputLayout.h"

std::vector<Vertex> Cube::s_vertices;
std::vector<Vertex> Cube::s_outlineVertices;
std::vector<unsigned short> Cube::s_indices;

Cube::Cube(const Game* game, const Graphics& graphics, const Vector3& translation, const Vector3& rotation, const Vector3& scale,
	bool isStatic)
	: Entity(game, translation, rotation, scale, s_indices.size(), isStatic), m_attributes{ 5.0f } {

	if (s_indices.empty()) {
		GenerateMesh(); // generate static vertices and indices
		GenerateOutline({ 1.0f, 0.4f, 0.4f, 1.0f });
	}
	m_indicesSize = s_indices.size(); // make sure size in Entity changes

	{
		// buffers & textures
		std::shared_ptr<GraphicsResource> pVertexBuffer = ResourcePool::GetResource<VertexBuffer>(
			graphics, s_vertices.data(), sizeof(VertexTexture), s_vertices.size(), VertexBuffer::Topology::Triangle, L"Cube");
		m_graphicsResources.push_back(std::move(pVertexBuffer));

		std::shared_ptr<GraphicsResource> pIndexBuffer = ResourcePool::GetResource<IndexBuffer>(
			graphics, s_indices, L"Cube");
		m_graphicsResources.push_back(std::move(pIndexBuffer));

		AddShadingResource(graphics);

		m_graphicsResources.push_back(std::make_shared<ConstantBufferTransformation>(graphics, *this));
		m_graphicsResources.push_back(std::make_shared<ConstantBufferVertex<Attributes>>(
			graphics, m_attributes, VertexConstantBufferType::Attributes, GetUID()));
		m_graphicsResources.push_back(std::make_shared<ConstantBufferPixel<Attributes>>(
			graphics, m_attributes, PixelConstantBufferType::Attributes, GetUID()));

		m_graphicsResources.push_back(std::make_shared<DepthStencil>(graphics, DepthStencil::Mode::Write));
	}
	
	{
		// outline effect
		std::shared_ptr<GraphicsResource> pVertexBuffer = ResourcePool::GetResource<VertexBuffer>(
			graphics, s_outlineVertices.data(), sizeof(VertexTexture), s_outlineVertices.size(), VertexBuffer::Topology::Triangle, L"CubeOutline");
		m_outlineResources.push_back(std::move(pVertexBuffer));

		std::shared_ptr<GraphicsResource> pIndexBuffer = ResourcePool::GetResource<IndexBuffer>(
			graphics, s_indices, L"Cube");
		m_outlineResources.push_back(std::move(pIndexBuffer));

		std::shared_ptr<GraphicsResource> pVertexShader = ResourcePool::GetResource<VertexShader>(
			graphics, L"BasicVS.cso");
		const std::vector<uint8_t> vertexShaderBlob = static_cast<VertexShader*>(pVertexShader.get())->GetVertexShaderBlob();
		m_outlineResources.push_back(std::move(pVertexShader));

		std::shared_ptr<GraphicsResource> pPixelShader = ResourcePool::GetResource<PixelShader>(
			graphics, L"BasicPS.cso");
		m_outlineResources.push_back(std::move(pPixelShader));

		std::shared_ptr<GraphicsResource> pInputLayout = ResourcePool::GetResource<InputLayout>(
			graphics, vertexShaderBlob, InputLayout::LayoutType::Shading);
		m_outlineResources.push_back(std::move(pInputLayout));

		m_outlineResources.push_back(std::make_shared<ConstantBufferTransformation>(graphics, *this));
		m_outlineResources.push_back(std::make_shared<DepthStencil>(graphics, DepthStencil::Mode::Mask));
	}
}

void Cube::DrawOutLine(const Graphics& graphics) {
	for (const std::shared_ptr<GraphicsResource>& resource : m_outlineResources) {
		resource->Bind(graphics);
	}

	graphics.RenderIndexed(m_indicesSize);
}

void Cube::GenerateMesh() {
	const float len = 0.5f;
	const DirectX::XMVECTOR verticesPosition[8] = {
		{DirectX::XMVectorSet(-len, -len, -len,  1.0f)},
		{DirectX::XMVectorSet( len, -len, -len,  1.0f)},
		{DirectX::XMVectorSet( len,  len, -len,  1.0f)},
		{DirectX::XMVectorSet(-len,  len, -len,  1.0f)},
		{DirectX::XMVectorSet(-len, -len,  len,  1.0f)},
		{DirectX::XMVectorSet( len, -len,  len,  1.0f)},
		{DirectX::XMVectorSet( len,  len,  len,  1.0f)},
		{DirectX::XMVectorSet(-len,  len,  len,  1.0f)},
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

void Cube::GenerateOutline(Color4 outlineColor) {
	const float len = 0.52f;
	const DirectX::XMVECTOR verticesPosition[8] = {
		{DirectX::XMVectorSet(-len, -len, -len,  1.0f)},
		{DirectX::XMVectorSet( len, -len, -len,  1.0f)},
		{DirectX::XMVectorSet( len,  len, -len,  1.0f)},
		{DirectX::XMVectorSet(-len,  len, -len,  1.0f)},
		{DirectX::XMVectorSet(-len, -len,  len,  1.0f)},
		{DirectX::XMVectorSet( len, -len,  len,  1.0f)},
		{DirectX::XMVectorSet( len,  len,  len,  1.0f)},
		{DirectX::XMVectorSet(-len,  len,  len,  1.0f)},
	};

	const unsigned short indices[24] = {
		0, 1, 2, 3, // back
		6, 5, 4, 7, // front
		7, 4, 0, 3, // left
		2, 1, 5, 6, // right
		4, 5, 1, 0, // bottom
		2, 6, 7, 3, // top
	};

	const Normal verticesNormal[6] = {
		{ 0.0f,  0.0f, -1.0f},
		{ 0.0f,  0.0f,  1.0f},
		{-1.0f,  0.0f,  0.0f},
		{ 1.0f,  0.0f,  0.0f},
		{ 0.0f, -1.0f,  0.0f},
		{ 0.0f,  1.0f,  0.0f},
	};

	s_outlineVertices.resize(24);
	for (int i = 0; i < 24; i++) {
		s_outlineVertices[i].m_position = verticesPosition[indices[i]];
		s_outlineVertices[i].m_color = outlineColor;
		s_outlineVertices[i].m_normal = verticesNormal[i / 4];
	}
}
