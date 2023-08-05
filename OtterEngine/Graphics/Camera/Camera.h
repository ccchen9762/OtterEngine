#pragma once

#include <DirectXMath.h>

#include "OtterEngine/Common/Math/Vector3.h"

class Camera
{
public:
	Camera(Vector3 position, float fov, float ratio, float near, float far);
	~Camera() = default;

	void MoveCamera(Vector3 position);
	void SetViewMatrix();
	void SetProjectionMatrix(float fov, float ratio, float near, float far);

	const DirectX::XMMATRIX& GetViewProjectionMatrix() const;

private:
	Vector3 m_position;
	DirectX::XMMATRIX m_viewMatrix;
	DirectX::XMMATRIX m_projectionMatrix;
	DirectX::XMMATRIX m_viewProjectionMatrix;
};