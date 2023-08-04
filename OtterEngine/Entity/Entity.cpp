#include "Entity.h"

Entity::Entity(Vector3 rotation, Vector3 translation, Vector3 revolution, Vector3 scale, size_t indicesSize) :
	m_rotation(rotation),
	m_translation(translation),
	m_revolution(revolution),
	m_scale(scale),
	m_indicesSize(indicesSize) {
}

void Entity::Render(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContext) {
	// need to use reference for unique pointer
	for (std::unique_ptr<GraphicsResource>& resource : m_graphicsBuffers) {
		resource->Bind(pDeviceContext.Get());
	}

	for (std::unique_ptr<GraphicsResource>& resource : m_graphicsResources) {
		resource->Bind(pDeviceContext.Get());
	}

	pDeviceContext->DrawIndexed(m_indicesSize, 0u, 0u); // draw with index buffer
}
