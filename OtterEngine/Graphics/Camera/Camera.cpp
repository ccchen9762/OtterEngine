#include "Camera.h"

Camera::Camera(Vector3 position, float fov, float ratio, float near, float far) :
	m_position(position) {

	m_viewMatrix = DirectX::XMMatrixTranslation(-position.x, -position.y, -position.z);
	SetProjectionMatrix(fov, ratio, near, far);
	m_viewProjectionMatrix = m_viewMatrix * m_projectionMatrix;
}

void Camera::MoveCamera(Vector3 position) {
	m_position += position;
	SetViewMatrix();
}

void Camera::SetViewMatrix() {
	m_viewMatrix = DirectX::XMMatrixTranslation(-m_position.x, -m_position.y, -m_position.z);
	m_viewProjectionMatrix = m_viewMatrix * m_projectionMatrix;
}

void Camera::SetProjectionMatrix(float fov, float ratio, float near, float far) {
	assert("near & far must be greater than 0" && near > 0 && far > 0);
	m_projectionMatrix = DirectX::XMMatrixPerspectiveFovRH(fov, ratio, near, far);
	m_viewProjectionMatrix = m_viewMatrix * m_projectionMatrix;
}

const DirectX::XMMATRIX& Camera::GetViewProjectionMatrix() const {
	return m_viewProjectionMatrix;
}
