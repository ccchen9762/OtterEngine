#include "Sphere.h"

#include "OtterEngine/Graphics/ResourcePool.h"
#include "OtterEngine/Graphics/Resource/VertexBuffer.h"
#include "OtterEngine/Graphics/Resource/IndexBuffer.h"
#include "OtterEngine/Common/constants.h"

std::vector<Vertex> Sphere::s_vertices;
std::vector<unsigned short> Sphere::s_indices;


Sphere::Sphere(const Game* game, const Graphics& graphics, const Vector3& translation, const Vector3& rotation, const Vector3& scale,
	bool isStatic)
	: Entity(game, translation, rotation, scale, s_indices.size(), isStatic), m_attributes{ 5.0f } {
	
	if (s_indices.empty()) {
		GenerateMesh(20); // generate static vertices and indices
	}
	m_indicesSize = s_indices.size(); // make sure size in Entity changes

	// buffers & textures
	std::shared_ptr<GraphicsResource> pVertexBuffer = ResourcePool::GetResource<VertexBuffer>(
		graphics, s_vertices.data(), sizeof(VertexTexture), s_vertices.size(), VertexBuffer::Topology::Triangle, L"Sphere");
	m_graphicsResources.push_back(std::move(pVertexBuffer));

	std::shared_ptr<GraphicsResource> pIndexBuffer = ResourcePool::GetResource<IndexBuffer>(
		graphics, s_indices, L"Sphere");
	m_graphicsResources.push_back(std::move(pIndexBuffer));

	AddShadingResource(graphics);

	m_graphicsResources.push_back(std::make_shared<ConstantBufferTransformation>(graphics, *this));
	m_graphicsResources.push_back(std::make_shared<ConstantBufferVertex<Attributes>>(
		graphics, m_attributes, VertexConstantBufferType::Attributes, GetUID()));
	m_graphicsResources.push_back(std::make_shared<ConstantBufferPixel<Attributes>>(
		graphics, m_attributes, PixelConstantBufferType::Attributes, GetUID()));
}

void Sphere::GenerateMesh(int division) {
	assert("division should be greater than 1" && division > 1);

	s_vertices.emplace_back(DirectX::XMVECTOR{ 0.0f, -0.5f, 0.0f, 1.0f }, Color4{ 1.0f, 1.0f, 1.0f, 1.0f }, Normal{ 0.0f, -1.0f, 0.0f });
	float stepAngle = kPI / division;
	float phi = -kPI / 2;
	float r = 0.5f;
	for (int i = 0; i < division-1; i++) {
		phi += stepAngle;
		float theta = -kPI / 2;
		for (int j = 0; j < division+1; j++) {
			theta += stepAngle * 2;
			float x = r * cosf(phi) * cosf(theta), y = r * sinf(phi), z = r * cosf(phi) * sinf(theta);
			s_vertices.emplace_back(DirectX::XMVECTOR{ x, y, z, 1.0f },
				Color4{ 1.0f, 1.0f, 1.0f, 1.0f },
				Normal{ x * 2, y * 2, z * 2 });
		}
	}
	s_vertices.emplace_back(DirectX::XMVECTOR{ 0.0f, 0.5f, 0.0f, 1.0f }, Color4{ 1.0f, 1.0f, 1.0f, 1.0f }, Normal{ 0.0f, 1.0f, 0.0f });

	int v1 = 0, v2 = 1;
	for (int i = 0; i < division; i++) {
		for (int j = 0; j < division; j++) {
			if (i == 0) {
				s_indices.push_back(v2 + 1);
				s_indices.push_back(v2);
				s_indices.push_back(v1);
				++v2;
			}
			else if (i == division-1) {
				s_indices.push_back(v2);
				s_indices.push_back(v1);
				s_indices.push_back(v1 + 1);
				++v1;
			}
			else {
				// top
				s_indices.push_back(v1);
				s_indices.push_back(v2 + 1);
				s_indices.push_back(v2);
				// down
				s_indices.push_back(v2 + 1);
				s_indices.push_back(v1);
				s_indices.push_back(v1 + 1);
				++v1, ++v2;
			}
		}
	}
}