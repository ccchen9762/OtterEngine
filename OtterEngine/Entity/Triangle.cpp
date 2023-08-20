#include "Triangle.h"

#include "OtterEngine/Graphics/Resource/VertexShader.h"
#include "OtterEngine/Graphics/Resource/PixelShader.h"
#include "OtterEngine/Graphics/Resource/InputLayout.h"
#include "OtterEngine/Graphics/Resource/VertexBuffer.h"
#include "OtterEngine/Graphics/Resource/IndexBuffer.h"
#include "OtterEngine/Graphics/Resource/ConstantBuffer.h"

const std::vector<VertexBasic> Triangle::s_vertices = {
	{DirectX::XMVectorSet( 0.0f,  0.5f,  0.0f, 1.0f), {1.0f, 0.0f, 0.0f, 1.0f}},
	{DirectX::XMVectorSet( 0.5f, -0.5f,  0.0f, 1.0f), {0.0f, 1.0f, 0.0f, 1.0f}},
	{DirectX::XMVectorSet(-0.5f, -0.5f,  0.0f, 1.0f), {0.0f, 0.0f, 1.0f, 1.0f}},
};

const std::vector<unsigned short> Triangle::s_indices = {
	0,1,2
};

std::vector<std::unique_ptr<GraphicsResource>> Triangle::s_commonResources;

Triangle::Triangle(const Graphics& graphics, const Vector3& translation, const Vector3& rotation, const Vector3& scale,
	const Camera& camera, bool isStatic)
	: Entity(translation, rotation, scale, s_indices.size(), camera, isStatic) {

	if (s_commonResources.empty()) {
		// shaders & layout
		std::unique_ptr<VertexShader> pVertexShader = std::make_unique<VertexShader>(graphics, L"GouraudVertexShader.cso");
		const std::vector<uint8_t> vertexShaderBlob = pVertexShader->GetVertexShaderBlob();
		s_commonResources.push_back(std::move(pVertexShader));
		s_commonResources.push_back(std::make_unique<PixelShader>(graphics, L"GouraudPixelShader.cso"));
		s_commonResources.push_back(std::make_unique<InputLayout>(graphics, vertexShaderBlob));

		// buffers
		s_commonResources.push_back(std::make_unique<VertexBuffer>(graphics, 
			s_vertices.data(), static_cast<unsigned int>(sizeof(VertexBasic)), s_vertices.size()));
		s_commonResources.push_back(std::make_unique<IndexBuffer>(graphics, s_indices));
	}

	m_uniqueResources.push_back(std::make_unique<ConstantBufferTransformation>(graphics, *this));
}

const std::vector<std::unique_ptr<GraphicsResource>>& Triangle::GetCommonResources() const {
	return s_commonResources;
}