#include "Entity.h"

Entity::Entity(Vector3 rotation, Vector3 translation, Vector3 revolution, Vector3 scale, size_t indicesSize, float speed) :
	m_rotation(rotation),
	m_translation(translation),
	m_revolution(revolution),
	m_scale(scale),
	m_indicesSize(indicesSize),
	m_speed(speed) {
}

void Entity::Update() {
	m_rotation.x += m_speed;
	m_rotation.y += m_speed;
	m_rotation.z += m_speed;
}

void Entity::Render(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContext) const {
	// need to use reference for unique pointer
	for (const std::unique_ptr<GraphicsResource>& resource : m_graphicsBuffers) {
		resource->Bind(pDeviceContext.Get());
	}

	for (const std::unique_ptr<GraphicsResource>& resource : m_graphicsResources) {
		resource->Bind(pDeviceContext.Get());
	}

	pDeviceContext->DrawIndexed(m_indicesSize, 0u, 0u); // draw with index buffer
}

DirectX::XMMATRIX Entity::GetTransformMatrix() const {

	return //DirectX::XMMatrixRotationRollPitchYaw(m_rotation.x, m_rotation.y, m_rotation.z) *
		DirectX::XMMatrixRotationX(m_rotation.x) *
		DirectX::XMMatrixRotationY(m_rotation.y) *
		DirectX::XMMatrixRotationZ(m_rotation.z) *
		DirectX::XMMatrixTranslation(m_translation.x, m_translation.y, m_translation.z) *
		//DirectX::XMMatrixRotationRollPitchYaw(m_revolution.x, m_revolution.y, m_revolution.z) *
		DirectX::XMMatrixScaling(m_scale.x, m_scale.y, m_scale.z);
}
