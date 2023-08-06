#include "Camera.h"

Camera::Camera(Vector3 position, Vector3 orientation, Vector3 up, float fov, float ratio, float nearZ, float farZ) :
	m_position(position),
	m_orientation(orientation),
	m_up(up) {

	m_viewMatrix = DirectX::XMMatrixTranslation(-position.x, -position.y, -position.z);
	SetProjectionMatrix(fov, ratio, nearZ, farZ);
	m_viewProjectionMatrix = m_viewMatrix * m_projectionMatrix;
}

void Camera::Update() {
	// move camera only when L CTRL is pressed
	/*if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
		double mouseX, mouseY;
		glfwGetCursorPos(window, &mouseX, &mouseY);

		// translation XY
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
			float translateX = m_Speed * (float)(mouseX - m_MousePositionX);
			float translateY = m_Speed * (float)(mouseY - m_MousePositionY);

			m_Position += translateX * glm::cross(m_Orientation, m_UpVector);
			m_Position -= translateY * m_UpVector;
			//m_Position.x += translateX;
			//m_Position.y -= translateY;
		}
		// translation Z
		else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS) {
			float translateY = m_Speed * (float)(mouseY - m_MousePositionY);

			m_Position -= translateY * m_Orientation;
			//m_Position.z -= translateY;
		}
		// rotation
		else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
			double mouseX, mouseY;
			glfwGetCursorPos(window, &mouseX, &mouseY);

			float rotationX = m_Sensitivity * (float)(mouseY - m_MousePositionY);
			float rotationY = m_Sensitivity * (float)(mouseX - m_MousePositionX);

			glm::vec3 newOrientation = glm::rotate(m_Orientation, glm::radians(-rotationX), glm::normalize(glm::cross(m_Orientation, m_UpVector)));
			if (!(glm::angle(newOrientation, m_UpVector) <= glm::radians(5.0f) || glm::angle(newOrientation, -m_UpVector) <= glm::radians(5.0f))) {
				m_Orientation = newOrientation;
			}
			m_Orientation = glm::rotate(m_Orientation, glm::radians(-rotationY), m_UpVector);
		}

		m_MousePositionX = mouseX;
		m_MousePositionY = mouseY;
	}

	glfwGetCursorPos(window, &m_MousePositionX, &m_MousePositionY);*/
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
