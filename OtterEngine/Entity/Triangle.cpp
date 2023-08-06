#include "Triangle.h"

#include "OtterEngine/Graphics/Resource/VertexShader.h"
#include "OtterEngine/Graphics/Resource/PixelShader.h"
#include "OtterEngine/Graphics/Resource/InputLayout.h"
#include "OtterEngine/Graphics/Resource/VertexBuffer.h"
#include "OtterEngine/Graphics/Resource/IndexBuffer.h"
#include "OtterEngine/Graphics/Resource/ConstantBuffer.h"

const std::vector<Vertex> Triangle::s_vertices = {
	{DirectX::XMVectorSet(0.0f,  0.5f,  0.0f, 1.0f), DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f)},
	{DirectX::XMVectorSet(0.5f, -0.5f,  0.0f, 1.0f), DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f)},
	{DirectX::XMVectorSet(-0.5f, -0.5f,  0.0f, 1.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f)},
};

const std::vector<unsigned short> Triangle::s_indices = {
	0,1,2
};

std::vector<std::unique_ptr<GraphicsResource>> Triangle::s_commonResources;

Triangle::Triangle(const Graphics& graphics, Vector3 rotation, Vector3 translation, Vector3 revolution, Vector3 scale, 
	const DirectX::XMMATRIX& viewProjectionMatrix, float speed)
	: Entity(rotation, translation, revolution, scale, s_indices.size(), viewProjectionMatrix, speed) {

	if (s_commonResources.empty()) {
		// shaders & layout
		std::unique_ptr<VertexShader> pVertexShader = std::make_unique<VertexShader>(graphics);
		const std::vector<uint8_t> vertexShaderBlob = pVertexShader->GetVertexShaderBlob();
		s_commonResources.push_back(std::move(pVertexShader));
		s_commonResources.push_back(std::make_unique<PixelShader>(graphics));
		s_commonResources.push_back(std::make_unique<InputLayout>(graphics, vertexShaderBlob));

		// buffers
		s_commonResources.push_back(std::make_unique<VertexBuffer>(graphics, s_vertices));
		s_commonResources.push_back(std::make_unique<IndexBuffer>(graphics, s_indices));
	}

	m_uniqueResources.push_back(std::make_unique<ConstantBufferTransformation>(graphics, *this));
}

const std::vector<std::unique_ptr<GraphicsResource>>& Triangle::GetCommonResources() const {
	return s_commonResources;
}