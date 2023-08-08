#include "Entity.h"

Entity::Entity(Vector3 rotation, Vector3 translation, Vector3 revolution, Vector3 scale, size_t indicesSize, 
	const DirectX::XMMATRIX& viewProjectionMatrix, float speed) :
	m_rotation(rotation),
	m_translation(translation),
	m_revolution(revolution),
	m_scale(scale),
	m_indicesSize(indicesSize),
	m_viewProjectionMatrix(viewProjectionMatrix),
	m_speed(speed) {
}

void Entity::Update() {
	m_rotation.x += m_speed;
	m_rotation.y += m_speed;
	m_rotation.z += m_speed;
}

void Entity::Render(const Graphics& graphics) const {
	// need to use reference for unique pointer
	for (const std::unique_ptr<GraphicsResource>& resource : GetCommonResources()) {
		resource->Bind(graphics);
	}

	for (const std::unique_ptr<GraphicsResource>& resource : m_uniqueResources) {
		resource->Bind(graphics);
	}

	graphics.RenderIndexed(m_indicesSize);
}

DirectX::XMMATRIX Entity::GetTransformMatrix() const {

	return //DirectX::XMMatrixRotationRollPitchYaw(m_rotation.x, m_rotation.y, m_rotation.z) *
		DirectX::XMMatrixScaling(m_scale.x, m_scale.y, m_scale.z) *
		DirectX::XMMatrixRotationX(m_rotation.x) *
		DirectX::XMMatrixRotationY(m_rotation.y) *
		DirectX::XMMatrixRotationZ(m_rotation.z) *
		DirectX::XMMatrixTranslation(m_translation.x, m_translation.y, m_translation.z);
		//DirectX::XMMatrixRotationRollPitchYaw(m_revolution.x, m_revolution.y, m_revolution.z);
}

const DirectX::XMMATRIX& Entity::GetViewProjectionMatrix() const {
	return m_viewProjectionMatrix;
}
