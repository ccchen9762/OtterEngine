#include "DebugSphere.h"

#include "OtterEngine/Graphics/Resource/VertexShader.h"
#include "OtterEngine/Graphics/Resource/PixelShader.h"
#include "OtterEngine/Graphics/Resource/InputLayout.h"
#include "OtterEngine/Graphics/Resource/VertexBuffer.h"
#include "OtterEngine/Graphics/Resource/IndexBuffer.h"
#include "OtterEngine/Graphics/Resource/ConstantBuffer.h"

std::vector<VertexBasic> DebugSphere::s_vertices;
std::vector<unsigned short> DebugSphere::s_indices;

std::vector<std::unique_ptr<GraphicsResource>> DebugSphere::s_commonResources;

DebugSphere::DebugSphere(const Graphics& graphics, const Vector3& translation, const Vector3& rotation, const Vector3& scale,
	const Camera& camera, bool isStatic)
	: Entity(translation, rotation, scale, s_indices.size(), camera, isStatic) {

	if (s_commonResources.empty()) {
		GenerateMesh(20); // generate static vertices and indices

		// shaders & layout
		std::unique_ptr<VertexShader> pVertexShader = std::make_unique<VertexShader>(graphics, L"BasicVertexShader.cso");
		const std::vector<uint8_t> vertexShaderBlob = pVertexShader->GetVertexShaderBlob();
		s_commonResources.push_back(std::move(pVertexShader));
		s_commonResources.push_back(std::make_unique<PixelShader>(graphics, L"BasicPixelShader.cso"));
		s_commonResources.push_back(std::make_unique<InputLayout>(graphics, vertexShaderBlob));

		// buffers
		s_commonResources.push_back(std::make_unique<VertexBuffer>(graphics,
			s_vertices.data(), static_cast<unsigned int>(sizeof(VertexBasic)), s_vertices.size()));
		s_commonResources.push_back(std::make_unique<IndexBuffer>(graphics, s_indices));
	}

	m_uniqueResources.push_back(std::make_unique<ConstantBufferTransformation>(graphics, *this));
}

void DebugSphere::GenerateMesh(int division) {
	assert("division should be greater than 1" && division > 1);

	s_vertices.emplace_back(DirectX::XMVECTOR{ 0.0f, -0.5f, 0.0f, 1.0f }, Color4{ 1.0f, 1.0f, 1.0f, 1.0f });
	float stepAngle = kPI / division;
	float phi = -kPI / 2;
	float r = 0.5f;
	for (int i = 0; i < division - 1; i++) {
		phi += stepAngle;
		float theta = -kPI / 2;
		for (int j = 0; j < division + 1; j++) {
			theta += stepAngle * 2;
			float x = r * cosf(phi) * cosf(theta), y = r * sinf(phi), z = r * cosf(phi) * sinf(theta);
			s_vertices.emplace_back(DirectX::XMVECTOR{ x, y, z, 1.0f }, Color4{ 1.0f, 1.0f, 1.0f, 1.0f });
		}
	}
	s_vertices.emplace_back(DirectX::XMVECTOR{ 0.0f, 0.5f, 0.0f, 1.0f }, Color4{ 1.0f, 1.0f, 1.0f, 1.0f });

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

const std::vector<std::unique_ptr<GraphicsResource>>& DebugSphere::GetCommonResources() const {
	return s_commonResources;
}