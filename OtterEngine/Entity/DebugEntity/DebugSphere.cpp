#include "DebugSphere.h"

#include "OtterEngine/Graphics/ResourcePool.h"
#include "OtterEngine/Graphics/Resource/VertexShader.h"
#include "OtterEngine/Graphics/Resource/PixelShader.h"
#include "OtterEngine/Graphics/Resource/InputLayout.h"
#include "OtterEngine/Graphics/Resource/VertexBuffer.h"
#include "OtterEngine/Graphics/Resource/IndexBuffer.h"
#include "OtterEngine/Common/constants.h"

std::vector<VertexBasic> DebugSphere::s_vertices;
std::vector<unsigned short> DebugSphere::s_indices;

DebugSphere::DebugSphere(const Game& game, const Graphics& graphics, const Vector3& translation, const Vector3& rotation, const Vector3& scale,
	bool isStatic)
	: Entity(game, translation, rotation, scale, s_indices.size(), isStatic) {

	GenerateMesh(20);

	// buffers
	std::shared_ptr<GraphicsResource> pVertexBuffer = ResourcePool::GetResource<VertexBuffer>(
		graphics, s_vertices.data(), sizeof(VertexBasic), s_vertices.size(), VertexBuffer::Topology::Triangle, L"DebugSphere");
	m_graphicsResources.push_back(std::move(pVertexBuffer));

	std::shared_ptr<GraphicsResource> pIndexBuffer = ResourcePool::GetResource<IndexBuffer>(
		graphics, s_indices, L"DebugSphere");
	m_graphicsResources.push_back(std::move(pIndexBuffer));

	std::shared_ptr<GraphicsResource> pConstantBufferTransformation = ResourcePool::GetResource<ConstantBufferTransformation>(
		graphics, *this);
	m_graphicsResources.push_back(std::move(pConstantBufferTransformation));

	// shaders & layout
	std::shared_ptr<GraphicsResource> pVertexShader = ResourcePool::GetResource<VertexShader>(
		graphics, L"BasicVS.cso");
	const std::vector<uint8_t> vertexShaderBlob = static_cast<VertexShader*>(pVertexShader.get())->GetVertexShaderBlob();
	m_graphicsResources.push_back(std::move(pVertexShader));

	std::shared_ptr<GraphicsResource> pPixelShader = ResourcePool::GetResource<PixelShader>(
		graphics, L"BasicPS.cso");
	m_graphicsResources.push_back(std::move(pPixelShader));

	std::shared_ptr<GraphicsResource> pInputLayout = ResourcePool::GetResource<InputLayout>(
		graphics, vertexShaderBlob, InputLayout::LayoutType::Basic);
	m_graphicsResources.push_back(std::move(pInputLayout));

	std::shared_ptr<GraphicsResource> pConstantBufferPixel = ResourcePool::GetResource<ConstantBufferPixel<Color4>>(
		graphics, Color4{ 1.0f, 1.0f, 1.0f, 1.0f }, PixelConstantBufferType::Attributes, GetUID());
	m_graphicsResources.push_back(std::move(pConstantBufferPixel));
}

void DebugSphere::GenerateMesh(int division) {
	assert("division should be greater than 1" && division > 1);

	s_vertices.emplace_back(DirectX::XMVECTOR{ 0.0f, -0.5f, 0.0f, 1.0f });
	float stepAngle = kPI / division;
	float phi = -kPI / 2;
	float r = 0.5f;
	for (int i = 0; i < division - 1; i++) {
		phi += stepAngle;
		float theta = -kPI / 2;
		for (int j = 0; j < division + 1; j++) {
			theta += stepAngle * 2;
			float x = r * cosf(phi) * cosf(theta), y = r * sinf(phi), z = r * cosf(phi) * sinf(theta);
			s_vertices.emplace_back(DirectX::XMVECTOR{ x, y, z, 1.0f });
		}
	}
	s_vertices.emplace_back(DirectX::XMVECTOR{ 0.0f, 0.5f, 0.0f, 1.0f });

	int v1 = 0, v2 = 1;
	for (int i = 0; i < division; i++) {
		for (int j = 0; j < division; j++) {
			if (i == 0) {
				s_indices.push_back(v2 + 1);
				s_indices.push_back(v2);
				s_indices.push_back(v1);
				++v2;
			}
			else if (i == division - 1) {
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

	// remember to change the size
	m_indicesSize = s_indices.size();
}