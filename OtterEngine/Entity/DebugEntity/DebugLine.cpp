#include "DebugLine.h"

#include "OtterEngine/Graphics/ResourcePool.h"
#include "OtterEngine/Graphics/Resource/VertexShader.h"
#include "OtterEngine/Graphics/Resource/PixelShader.h"
#include "OtterEngine/Graphics/Resource/InputLayout.h"
#include "OtterEngine/Graphics/Resource/VertexBuffer.h"
#include "OtterEngine/Graphics/Resource/IndexBuffer.h"

const std::vector<unsigned short> DebugLine::s_indices = {
	0,1
};

DebugLine::DebugLine(const Game& game, const Graphics& graphics, const Vector3& translation, const Vector3& rotation, const Vector3& scale,
	const Color4& color, bool isStatic)
	: Entity(game, translation, rotation, scale, s_indices.size(), isStatic) {

	m_vertices = {
		{DirectX::XMVectorSet(0.0f,  0.0f,  0.0f, 1.0f)},
		{DirectX::XMVectorSet(1.0f,  0.0f,  0.0f, 1.0f)},
	};

	// buffers
	std::shared_ptr<GraphicsResource> pVertexBuffer = ResourcePool::GetResource<VertexBuffer>(
		graphics, m_vertices.data(), sizeof(VertexBasic), m_vertices.size(), VertexBuffer::Topology::Line, L"DebugLine");
	m_graphicsResources.push_back(std::move(pVertexBuffer));

	std::shared_ptr<GraphicsResource> pIndexBuffer = ResourcePool::GetResource<IndexBuffer>(
		graphics, s_indices, L"DebugLine");
	m_graphicsResources.push_back(std::move(pIndexBuffer));

	AddBasicResource(graphics);

	m_graphicsResources.push_back(std::make_shared<ConstantBufferTransformation>(graphics, *this));
	m_graphicsResources.push_back(std::make_shared<ConstantBufferPixel<Color4>>(
		graphics, Color4{ color.r, color.g, color.b, color.a }, PixelConstantBufferType::Attributes, GetUID()));
}