#include "DebugLine.h"

#include "OtterEngine/Graphics/Resource/VertexShader.h"
#include "OtterEngine/Graphics/Resource/PixelShader.h"
#include "OtterEngine/Graphics/Resource/InputLayout.h"
#include "OtterEngine/Graphics/Resource/VertexBuffer.h"
#include "OtterEngine/Graphics/Resource/IndexBuffer.h"
#include "OtterEngine/Graphics/Resource/ConstantBuffer.h"

const std::vector<unsigned short> DebugLine::s_indices = {
	0,1
};

std::vector<std::unique_ptr<GraphicsResource>> DebugLine::s_commonResources;

DebugLine::DebugLine(const Graphics& graphics, const Vector3& translation, const Vector3& rotation, const Vector3& scale,
	const Camera& camera, const Color4& color, bool isStatic)
	: Entity(translation, rotation, scale, s_indices.size(), camera, isStatic) {

	m_vertices = {
		{DirectX::XMVectorSet(0.0f,  0.0f,  0.0f, 1.0f), {color.r, color.g, color.b, color.a}},
		{DirectX::XMVectorSet(1.0f,  0.0f,  0.0f, 1.0f), {color.r, color.g, color.b, color.a}},
	};

	if (s_commonResources.empty()) {
		// shaders & layout
		std::unique_ptr<VertexShader> pVertexShader = std::make_unique<VertexShader>(graphics, L"BasicVertexShader.cso");
		const std::vector<uint8_t> vertexShaderBlob = pVertexShader->GetVertexShaderBlob();
		s_commonResources.push_back(std::move(pVertexShader));
		s_commonResources.push_back(std::make_unique<PixelShader>(graphics, L"BasicPixelShader.cso"));
		s_commonResources.push_back(std::make_unique<InputLayout>(graphics, vertexShaderBlob));

		// buffers
		s_commonResources.push_back(std::make_unique<IndexBuffer>(graphics, s_indices));
	}

	m_uniqueResources.push_back(std::make_unique<VertexBuffer>(graphics,
		m_vertices.data(), static_cast<unsigned int>(sizeof(VertexBasic)), m_vertices.size(), false)); // make unique since m_vertices are different

	m_uniqueResources.push_back(std::make_unique<ConstantBufferTransformation>(graphics, *this));
}

const std::vector<std::unique_ptr<GraphicsResource>>& DebugLine::GetCommonResources() const {
	return s_commonResources;
}