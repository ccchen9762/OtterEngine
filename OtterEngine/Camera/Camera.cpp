#include "Camera.h"

Camera::Camera(const Graphics& graphics, const Vector3& position, const Vector3& orientation, const Vector3& up,
	float fov, float ratio, float nearZ, float farZ, float speed, float angularSpeed) :
	m_position(position), m_orientation(orientation), m_up(up),
	m_speed(speed), m_angularSpeed(angularSpeed),
	m_cameraBuffer({ DirectX::XMVECTOR{ m_position.x, m_position.y, m_position.z, 1.0f } }),
	m_constantBufferVertex(graphics, m_cameraBuffer, VertexConstantBufferType::Camera),
	m_constantBufferPixel(graphics, m_cameraBuffer, PixelConstantBufferType::Camera) {

	SetViewMatrix();
	SetProjectionMatrix(fov, ratio, nearZ, farZ);
	m_viewProjectionMatrix = m_viewMatrix * m_projectionMatrix;
}

void Camera::Update(const Graphics& graphics) const {
	m_constantBufferVertex.Update(graphics, m_cameraBuffer);
	m_constantBufferPixel.Update(graphics, m_cameraBuffer);
}

void Camera::TranslateCamera(const Vector3Int& position, const Vector3Int& prevPosition) {
	float translateX = m_speed * (position.x - prevPosition.x);
	float translateY = m_speed * (position.y - prevPosition.y);

	m_position += translateX * CrossProduct(m_orientation, m_up);
	m_position -= translateY * m_up;

	SetViewMatrix();
}

void Camera::TranslateCameraZ(bool wheelUp) {
	float translateZ = wheelUp ? 1.0f : -1.0f;
	translateZ *= m_speed * 50.0f;

	m_position += translateZ * m_orientation;

	SetViewMatrix();
}

void Camera::RotateCamera(const Vector3Int& position, const Vector3Int& prevPosition) {
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

	m_cameraBuffer.position = DirectX::XMVECTOR{ m_position.x, m_position.y, m_position.z, 1.0f };
}

void Camera::SetProjectionMatrix(float fov, float ratio, float nearZ, float farZ) {
	assert("near & far must be greater than 0" && nearZ > 0 && farZ > 0);
	m_projectionMatrix = DirectX::XMMatrixPerspectiveFovRH(fov, ratio, nearZ, farZ);
	m_viewProjectionMatrix = m_viewMatrix * m_projectionMatrix;
}

const DirectX::XMMATRIX& Camera::GetViewMatrix() const {
	return m_viewMatrix;
}

const DirectX::XMMATRIX& Camera::GetViewProjectionMatrix() const {
	return m_viewProjectionMatrix;
}
