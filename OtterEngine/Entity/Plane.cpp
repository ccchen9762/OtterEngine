#include "Plane.h"

#include "OtterEngine/Graphics/Resource/VertexShader.h"
#include "OtterEngine/Graphics/Resource/PixelShader.h"
#include "OtterEngine/Graphics/Resource/InputLayout.h"
#include "OtterEngine/Graphics/Resource/VertexBuffer.h"
#include "OtterEngine/Graphics/Resource/IndexBuffer.h"
#include "OtterEngine/Graphics/Resource/ConstantBuffer.h"
#include "OtterEngine/Graphics/Resource/Texture.h"

const std::vector<VertexTexture> Plane::s_vertices = {
	{DirectX::XMVectorSet(-0.5f, 0.0f,  0.5f,  1.0f), {1.0f, 1.0f}},
	{DirectX::XMVectorSet(-0.5f, 0.0f, -0.5f,  1.0f), {1.0f, 0.0f}},
	{DirectX::XMVectorSet( 0.5f, 0.0f, -0.5f,  1.0f), {0.0f, 0.0f}},
	{DirectX::XMVectorSet( 0.5f, 0.0f,  0.5f,  1.0f), {0.0f, 1.0f}},
};

const std::vector<unsigned short> Plane::s_indices = {
	0, 1, 2,  2, 3, 0
};

std::vector<std::unique_ptr<GraphicsResource>> Plane::s_commonResources;

Plane::Plane(const Graphics& graphics, const Vector3& translation, const Vector3& rotation, const Vector3& scale,
	const DirectX::XMMATRIX& viewProjectionMatrix, const std::wstring& path, bool isStatic)
	: Entity(translation, rotation, scale, s_indices.size(), viewProjectionMatrix, isStatic) {

	if (s_commonResources.empty()) {
		// shaders & layout
		std::unique_ptr<VertexShader> pVertexShader = std::make_unique<VertexShader>(graphics, L"VertexShaderTexture.cso");
		const std::vector<uint8_t> vertexShaderBlob = pVertexShader->GetVertexShaderBlob();
		s_commonResources.push_back(std::move(pVertexShader));
		s_commonResources.push_back(std::make_unique<PixelShader>(graphics, L"PixelShaderTexture.cso"));
		s_commonResources.push_back(std::make_unique<InputLayout>(graphics, vertexShaderBlob, true));

		// buffers
		s_commonResources.push_back(std::make_unique<VertexBuffer>(graphics, 
			s_vertices.data(), static_cast<unsigned int>(sizeof(VertexTexture)), s_vertices.size()));
		s_commonResources.push_back(std::make_unique<IndexBuffer>(graphics, s_indices));
	}

	m_uniqueResources.push_back(std::make_unique<Texture>(graphics, path));
	m_uniqueResources.push_back(std::make_unique<ConstantBufferTransformation>(graphics, *this));
}

const std::vector<std::unique_ptr<GraphicsResource>>& Plane::GetCommonResources() const {
	return s_commonResources;
}
