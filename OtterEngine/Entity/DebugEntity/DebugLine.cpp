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
		graphics, Color4{ color.r, color.g, color.b, color.a }, PixelConstantBufferType::Attributes, GetUID());
	m_graphicsResources.push_back(std::move(pConstantBufferPixel));
}