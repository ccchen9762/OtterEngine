#include "DebugEntity.h"

#include "OtterEngine/Graphics/Resource/VertexShader.h"
#include "OtterEngine/Graphics/Resource/PixelShader.h"
#include "OtterEngine/Graphics/Resource/InputLayout.h"

std::vector<std::unique_ptr<GraphicsResource>> DebugEntity::s_shadingResources;

DebugEntity::DebugEntity(const Graphics& graphics, const Vector3& translation, const Vector3& rotation, const Vector3& scale,
	size_t indiceSize, const Camera& camera, bool isStatic)
	: Entity(translation, rotation, scale, indiceSize, camera, isStatic) {

	if (s_shadingResources.empty()) {
		// shaders & layout
		std::unique_ptr<VertexShader> pVertexShader = std::make_unique<VertexShader>(graphics, L"BasicVertexShader.cso");
		const std::vector<uint8_t> vertexShaderBlob = pVertexShader->GetVertexShaderBlob();
		s_shadingResources.push_back(std::move(pVertexShader));
		s_shadingResources.push_back(std::make_unique<PixelShader>(graphics, L"BasicPixelShader.cso"));
		s_shadingResources.push_back(std::make_unique<InputLayout>(graphics, vertexShaderBlob));
	}

	m_uniqueResources.push_back(std::make_unique<ConstantBufferTransformation>(graphics, *this));
}
