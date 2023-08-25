#include "ShadingTexture.h"

#include "OtterEngine/Graphics/Resource/VertexShader.h"
#include "OtterEngine/Graphics/Resource/PixelShader.h"
#include "OtterEngine/Graphics/Resource/InputLayout.h"

std::vector<std::unique_ptr<GraphicsResource>> ShadingTexture::s_shadingResources;

ShadingTexture::ShadingTexture(const Graphics& graphics, const Vector3& translation, const Vector3& rotation, const Vector3& scale,
	size_t indiceSize, const Camera& camera, bool isStatic, Attributes attributes)
	: Entity(translation, rotation, scale, indiceSize, camera, isStatic), m_attributes(attributes) {

	if (s_shadingResources.empty()) {
		// shaders & layout
		if (kRenderMethod == RenderMethod::Gouraud) {
			std::unique_ptr<VertexShader> pVertexShader = std::make_unique<VertexShader>(graphics, L"TextureGouraudVertexShader.cso");
			const std::vector<uint8_t> vertexShaderBlob = pVertexShader->GetVertexShaderBlob();
			s_shadingResources.push_back(std::move(pVertexShader));
			s_shadingResources.push_back(std::make_unique<PixelShader>(graphics, L"TextureGouraudPixelShader.cso"));
			s_shadingResources.push_back(std::make_unique<InputLayout>(graphics, vertexShaderBlob, InputLayout::LayoutType::TextureShading));
		}
		else if (kRenderMethod == RenderMethod::Phong) {
			std::unique_ptr<VertexShader> pVertexShader = std::make_unique<VertexShader>(graphics, L"TexturePhongVertexShader.cso");
			const std::vector<uint8_t> vertexShaderBlob = pVertexShader->GetVertexShaderBlob();
			s_shadingResources.push_back(std::move(pVertexShader));
			s_shadingResources.push_back(std::make_unique<PixelShader>(graphics, L"TexturePhongPixelShader.cso"));
			s_shadingResources.push_back(std::make_unique<InputLayout>(graphics, vertexShaderBlob, InputLayout::LayoutType::TextureShading));
		}
	}

	m_uniqueResources.push_back(std::make_unique<ConstantBufferVertex<Attributes>>(graphics, m_attributes, VertexConstantBufferType::Attributes));
	m_uniqueResources.push_back(std::make_unique<ConstantBufferPixel<Attributes>>(graphics, m_attributes, PixelConstantBufferType::Attributes));
	m_uniqueResources.push_back(std::make_unique<ConstantBufferTransformation>(graphics, *this));
}
