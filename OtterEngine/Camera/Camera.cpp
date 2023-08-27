#include "Camera.h"

#include <string>

#include "OtterEngine/Common/constants.h"

int Camera::s_numCamera = 0;

Camera::Camera(const Graphics& graphics, const Vector3& position, const Vector3& orientation, const Vector3& up) :
	m_position(position), m_orientation(orientation), m_up(up),
	m_speed(0.02f), m_angularSpeed(0.1f),
	m_cameraBuffer({ DirectX::XMVECTOR{ m_position.x, m_position.y, m_position.z, 1.0f } }),
	m_constantBufferVertex(graphics, m_cameraBuffer, VertexConstantBufferType::Camera, L"Camera" + std::to_wstring(s_numCamera)),
	m_constantBufferPixel(graphics, m_cameraBuffer, PixelConstantBufferType::Camera, L"Camera" + std::to_wstring(s_numCamera)) {

	++s_numCamera;

	SetViewMatrix();
	SetProjectionMatrix(DirectX::XM_PIDIV4, kRenderRatio, kNearZ, kFarZ);
	m_viewProjectionMatrix = m_viewMatrix * m_projectionMatrix;
}

void Camera::Update(const Graphics& graphics) {
	// generate new viewProjectionMatrix
	SetViewMatrix();

	// update constant buffers
	m_cameraBuffer.position = DirectX::XMVECTOR{ m_position.x, m_position.y, m_position.z, 1.0f };
	m_constantBufferVertex.Update(graphics, m_cameraBuffer);
	m_constantBufferPixel.Update(graphics, m_cameraBuffer);

	m_constantBufferVertex.Bind(graphics);
	m_constantBufferPixel.Bind(graphics);
}

void Camera::TranslateCamera(float translateX, float translateY) {
	m_position += translateX * m_speed * CrossProduct(m_orientation, m_up);
	m_position -= translateY * m_speed * m_up;
}

void Camera::TranslateCameraZ(int translateZ) {
	m_position += translateZ * m_speed * m_orientation;
}

void Camera::RotateCamera(float rotationY, float rotationX) {
	rotationX *= m_angularSpeed;
	rotationY *= m_angularSpeed;

	Vector3 xAxis = CrossProduct(m_orientation, m_up);
	xAxis.normalize();
	m_orientation = RotateAroundAxis(m_orientation, xAxis, DirectX::XMConvertToRadians(rotationX));
	m_orientation = RotateAroundAxis(m_orientation, m_up, DirectX::XMConvertToRadians(rotationY));
}

void Camera::SetViewMatrix() {
	m_viewMatrix = DirectX::XMMatrixLookToRH(
		DirectX::XMVECTOR{ m_position.x, m_position.y, m_position.z },
		DirectX::XMVECTOR{ m_orientation.x, m_orientation.y, m_orientation.z },
		DirectX::XMVECTOR{ m_up.x, m_up.y, m_up.z });
	m_viewProjectionMatrix = m_viewMatrix * m_projectionMatrix;
}

void Camera::SetProjectionMatrix(float fov, float ratio, float nearZ, float farZ) {
	assert("near & far must be greater than 0" && nearZ > 0 && farZ > 0);
	m_projectionMatrix = DirectX::XMMatrixPerspectiveFovRH(fov, ratio, nearZ, farZ);
}
