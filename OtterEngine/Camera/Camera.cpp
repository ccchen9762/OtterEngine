#include "Camera.h"

Camera::Camera(Vector3 position, Vector3 orientation, Vector3 up, 
	float fov, float ratio, float nearZ, float farZ, float speed, float angularSpeed) :
	m_position(position), m_orientation(orientation), m_up(up),
	m_speed(speed), m_angularSpeed(angularSpeed) {

	SetViewMatrix();
	SetProjectionMatrix(fov, ratio, nearZ, farZ);
	m_viewProjectionMatrix = m_viewMatrix * m_projectionMatrix;
}

void Camera::TranslateCamera(Vector3Int position, Vector3Int prevPosition) {
	float translateX = m_speed * (position.x - prevPosition.x);
	float translateY = m_speed * (position.y - prevPosition.y);

	m_position += translateX * CrossProduct(m_orientation, m_up);
	m_position -= translateY * m_up;

	SetViewMatrix();
}

void Camera::RotateCamera(Vector3Int position, Vector3Int prevPosition) {
	float rotationX = m_angularSpeed * (position.y - prevPosition.y);
	float rotationY = m_angularSpeed * (position.x - prevPosition.x);

	Vector3 xAxis = CrossProduct(m_orientation, m_up);
	xAxis.normalize();
	m_orientation = RotateAroundAxis(m_orientation, xAxis, DirectX::XMConvertToRadians(rotationX));
	m_orientation = RotateAroundAxis(m_orientation, m_up, DirectX::XMConvertToRadians(rotationY));

	SetViewMatrix();
}

void Camera::SetViewMatrix() {
	m_viewMatrix = DirectX::XMMatrixLookToRH(
		DirectX::XMVECTOR{ m_position.x, m_position.y, m_position.z },
		DirectX::XMVECTOR{ m_orientation.x, m_orientation.y, m_orientation.z },
		DirectX::XMVECTOR{ m_up.x, m_up.y, m_up.z });
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
