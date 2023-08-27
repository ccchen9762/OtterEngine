#include "Entity.h"

#include <string>

#include "OtterEngine/Graphics/ResourcePool.h"
#include "OtterEngine/Graphics/Resource/VertexShader.h"
#include "OtterEngine/Graphics/Resource/PixelShader.h"
#include "OtterEngine/Graphics/Resource/InputLayout.h"
#include "OtterEngine/Common/constants.h"
#include "OtterEngine/Common/Randomizer.h"

Entity::Entity(const Vector3& translation, const Vector3& rotation, const Vector3& scale, size_t indicesSize,
	const Camera& camera, bool isStatic) :
	m_translation(translation), m_rotation(rotation), m_scale(scale),
	m_indicesSize(indicesSize),
	m_camera(camera),
	m_isStatic(isStatic),
	m_speed(0.0f), //Randomizer::GetFloat(0.02f, 0.08f)
	m_transformation(DirectX::XMMatrixIdentity()){ 

	static unsigned int numEntities = 0;
	m_UID = L"Entity#" + std::to_wstring(numEntities++);
}

void Entity::Update() {
	if (!m_isStatic) {
		m_rotation.x += m_speed;
		m_rotation.y += m_speed;
		m_rotation.z += m_speed;
	}

	// transformation sequence: scale -> rotate -> translate
	m_transformation = DirectX::XMMatrixScaling(m_scale.x, m_scale.y, m_scale.z) *
		DirectX::XMMatrixRotationRollPitchYaw(m_rotation.x, m_rotation.y, m_rotation.z) *
		DirectX::XMMatrixTranslation(m_translation.x, m_translation.y, m_translation.z);
}

void Entity::Render(const Graphics& graphics) const {
	// use reference for unique pointer
	for (const std::shared_ptr<GraphicsResource>& resource : m_graphicsResources) {
		resource->Bind(graphics);
	}

	graphics.RenderIndexed(m_indicesSize);
}

void Entity::AddShadingResource(const Graphics& graphics) {
	// shaders & layout
	if (kRenderMethod == RenderMethod::Gouraud) {
		std::shared_ptr<GraphicsResource> pVertexShader = ResourcePool::GetResource<VertexShader>(
			graphics, L"GouraudVS.cso");
		const std::vector<uint8_t> vertexShaderBlob = static_cast<VertexShader*>(pVertexShader.get())->GetVertexShaderBlob();
		m_graphicsResources.push_back(std::move(pVertexShader));

		std::shared_ptr<GraphicsResource> pPixelShader = ResourcePool::GetResource<PixelShader>(
			graphics, L"GouraudPS.cso");
		m_graphicsResources.push_back(std::move(pPixelShader));

		std::shared_ptr<GraphicsResource> pInputLayout = ResourcePool::GetResource<InputLayout>(
			graphics, vertexShaderBlob, InputLayout::LayoutType::Shading);
		m_graphicsResources.push_back(std::move(pInputLayout));
	}
	else if (kRenderMethod == RenderMethod::Phong) {
		std::shared_ptr<GraphicsResource> pVertexShader = ResourcePool::GetResource<VertexShader>(
			graphics, L"PhongVS.cso");
		const std::vector<uint8_t> vertexShaderBlob = static_cast<VertexShader*>(pVertexShader.get())->GetVertexShaderBlob();
		m_graphicsResources.push_back(std::move(pVertexShader));

		std::shared_ptr<GraphicsResource> pPixelShader = ResourcePool::GetResource<PixelShader>(
			graphics, L"PhongPS.cso");
		m_graphicsResources.push_back(std::move(pPixelShader));

		std::shared_ptr<GraphicsResource> pInputLayout = ResourcePool::GetResource<InputLayout>(
			graphics, vertexShaderBlob, InputLayout::LayoutType::Shading);
		m_graphicsResources.push_back(std::move(pInputLayout));
	}
}

void Entity::AddTextureShadingResource(const Graphics& graphics) {
	// shaders & layout
	if (kRenderMethod == RenderMethod::Gouraud) {
		std::shared_ptr<GraphicsResource> pVertexShader = ResourcePool::GetResource<VertexShader>(
			graphics, L"TextureGouraudVS.cso");
		const std::vector<uint8_t> vertexShaderBlob = static_cast<VertexShader*>(pVertexShader.get())->GetVertexShaderBlob();
		m_graphicsResources.push_back(std::move(pVertexShader));

		std::shared_ptr<GraphicsResource> pPixelShader = ResourcePool::GetResource<PixelShader>(
			graphics, L"TextureGouraudPS.cso");
		m_graphicsResources.push_back(std::move(pPixelShader));

		std::shared_ptr<GraphicsResource> pInputLayout = ResourcePool::GetResource<InputLayout>(
			graphics, vertexShaderBlob, InputLayout::LayoutType::TextureShading);
		m_graphicsResources.push_back(std::move(pInputLayout));
	}
	else if (kRenderMethod == RenderMethod::Phong) {
		std::shared_ptr<GraphicsResource> pVertexShader = ResourcePool::GetResource<VertexShader>(
			graphics, L"TexturePhongVS.cso");
		const std::vector<uint8_t> vertexShaderBlob = static_cast<VertexShader*>(pVertexShader.get())->GetVertexShaderBlob();
		m_graphicsResources.push_back(std::move(pVertexShader));

		std::shared_ptr<GraphicsResource> pPixelShader = ResourcePool::GetResource<PixelShader>(
			graphics, L"TexturePhongPS.cso");
		m_graphicsResources.push_back(std::move(pPixelShader));

		std::shared_ptr<GraphicsResource> pInputLayout = ResourcePool::GetResource<InputLayout>(
			graphics, vertexShaderBlob, InputLayout::LayoutType::TextureShading);
		m_graphicsResources.push_back(std::move(pInputLayout));
	}
}
