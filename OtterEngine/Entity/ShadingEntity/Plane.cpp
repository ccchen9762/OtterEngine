#include "Plane.h"

#include "OtterEngine/Graphics/ResourcePool.h"
#include "OtterEngine/Graphics/Resource/VertexBuffer.h"
#include "OtterEngine/Graphics/Resource/IndexBuffer.h"
#include "OtterEngine/Graphics/Resource/Texture.h"
#include "OtterEngine/Common/constants.h"

const std::vector<VertexTexture> Plane::s_vertices = {
	{DirectX::XMVectorSet(-0.5f, 0.0f,  0.5f,  1.0f), {1.0f, 1.0f}, {0.0f, 1.0f, 0.0f}},
	{DirectX::XMVectorSet(-0.5f, 0.0f, -0.5f,  1.0f), {1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},
	{DirectX::XMVectorSet( 0.5f, 0.0f, -0.5f,  1.0f), {0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},
	{DirectX::XMVectorSet( 0.5f, 0.0f,  0.5f,  1.0f), {0.0f, 1.0f}, {0.0f, 1.0f, 0.0f}},
};

const std::vector<unsigned short> Plane::s_indices = {
	0, 1, 2,  2, 3, 0
};

Plane::Plane(const Game& game, const Graphics& graphics, const Vector3& translation, const Vector3& rotation, const Vector3& scale,
	const std::wstring& path, bool isStatic)
	: Entity(game, translation, rotation, scale, s_indices.size(), isStatic), m_attributes{5.0f} {

	// buffers & textures
	std::shared_ptr<GraphicsResource> pVertexBuffer = ResourcePool::GetResource<VertexBuffer>(
		graphics, s_vertices.data(), sizeof(VertexTexture), s_vertices.size(), VertexBuffer::Topology::Triangle, L"Plane");
	m_graphicsResources.push_back(std::move(pVertexBuffer));

	std::shared_ptr<GraphicsResource> pIndexBuffer = ResourcePool::GetResource<IndexBuffer>(
		graphics, s_indices, L"Plane");
	m_graphicsResources.push_back(std::move(pIndexBuffer));

	std::shared_ptr<GraphicsResource> pTexture = ResourcePool::GetResource<Texture>(
		graphics, path, 0u);
	m_graphicsResources.push_back(std::move(pTexture));

	AddTextureShadingResource(graphics, false);

	m_graphicsResources.push_back(std::make_shared<ConstantBufferTransformation>(graphics, *this));
	m_graphicsResources.push_back(std::make_shared<ConstantBufferVertex<Attributes>>(
		graphics, m_attributes, VertexConstantBufferType::Attributes, GetUID()));
	m_graphicsResources.push_back(std::make_shared<ConstantBufferPixel<Attributes>>(
		graphics, m_attributes, PixelConstantBufferType::Attributes, GetUID()));
}