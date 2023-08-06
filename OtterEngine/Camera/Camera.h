#pragma once

#include <DirectXMath.h>

#include "OtterEngine/Common/Math/Vector3.h"

class Camera
{
public:
	Camera(Vector3 position, Vector3 orientation, Vector3 up, float fov, float ratio, float nearZ, float farZ);
	~Camera() = default;

	void Update();
	void MoveCamera(Vector3 position);
	void SetViewMatrix();
	void SetProjectionMatrix(float fov, float ratio, float near, float far);

	const DirectX::XMMATRIX& GetViewProjectionMatrix() const;

private:
	float m_sensitivity;
	Vector3 m_position;
	Vector3 m_orientation;
	Vector3 m_up;
	Vector3Int m_mousePositionX, m_mousePositionY;
	DirectX::XMMATRIX m_viewMatrix;
	DirectX::XMMATRIX m_projectionMatrix;
	DirectX::XMMATRIX m_viewProjectionMatrix;
};