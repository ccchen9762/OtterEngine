#include "Plane.h"

#include "OtterEngine/Graphics/Resource/VertexBuffer.h"
#include "OtterEngine/Graphics/Resource/IndexBuffer.h"
#include "OtterEngine/Graphics/Resource/Texture.h"

const std::vector<VertexTexture> Plane::s_vertices = {
	{DirectX::XMVectorSet(-0.5f, 0.0f,  0.5f,  1.0f), {1.0f, 1.0f}, {0.0f, 1.0f, 0.0f}},
	{DirectX::XMVectorSet(-0.5f, 0.0f, -0.5f,  1.0f), {1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},
	{DirectX::XMVectorSet( 0.5f, 0.0f, -0.5f,  1.0f), {0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},
	{DirectX::XMVectorSet( 0.5f, 0.0f,  0.5f,  1.0f), {0.0f, 1.0f}, {0.0f, 1.0f, 0.0f}},
};

const std::vector<unsigned short> Plane::s_indices = {
	0, 1, 2,  2, 3, 0
};

std::vector<std::unique_ptr<GraphicsResource>> Plane::s_commonResources;

Plane::Plane(const Graphics& graphics, const Vector3& translation, const Vector3& rotation, const Vector3& scale,
	const Camera& camera, const std::wstring& path, bool isStatic)
	: ShadingTexture(graphics, translation, rotation, scale, s_indices.size(), camera, isStatic) {

	if (s_commonResources.empty()) {
		// buffers
		s_commonResources.push_back(std::make_unique<VertexBuffer>(graphics, 
			s_vertices.data(), static_cast<unsigned int>(sizeof(VertexTexture)), s_vertices.size()));
		s_commonResources.push_back(std::make_unique<IndexBuffer>(graphics, s_indices));
	}

	m_uniqueResources.push_back(std::make_unique<Texture>(graphics, path));
}