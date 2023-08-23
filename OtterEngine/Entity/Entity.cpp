#include "Entity.h"

#include "OtterEngine/Common/Randomizer.h"

Entity::Entity(const Vector3& translation, const Vector3& rotation, const Vector3& scale, size_t indicesSize,
	const Camera& camera, bool isStatic) :
	m_translation(translation),
	m_rotation(rotation),
	m_scale(scale),
	m_indicesSize(indicesSize),
	m_camera(camera),
	m_isStatic(isStatic),
	m_speed(0.0f),
	m_transformation(DirectX::XMMatrixIdentity()){ //Randomizer::GetFloat(0.02f, 0.08f)
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
	const std::vector<std::unique_ptr<GraphicsResource>>& shadingResource = GetShadingResources();
	for (const std::unique_ptr<GraphicsResource>& resource : shadingResource) {
		resource->Bind(graphics);
	}

	const std::vector<std::unique_ptr<GraphicsResource>>& commonResource = GetCommonResources();
	for (const std::unique_ptr<GraphicsResource>& resource : commonResource) {
		resource->Bind(graphics);
	}

	for (const std::unique_ptr<GraphicsResource>& resource : m_uniqueResources) {
		resource->Bind(graphics);
	}

	graphics.RenderIndexed(m_indicesSize);
}
