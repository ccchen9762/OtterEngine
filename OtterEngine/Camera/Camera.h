#pragma once

#include <DirectXMath.h>

#include "OtterEngine/Graphics/Resource/ConstantBuffer.h"
#include "OtterEngine/Common/Math/MathUtils.h"

class Camera
{
private:
	struct CameraBuffer {
		DirectX::XMVECTOR position;
	};

public:
	Camera(const Graphics& graphics, const Vector3& position, const Vector3& orientation, const Vector3& up,
		float fov, float ratio, float nearZ, float farZ, float speed, float angularSpeed);
	~Camera() = default;
	
	void Update(const Graphics& graphics) const;

	void TranslateCamera(const Vector3Int& position, const Vector3Int& prevPosition);
	void TranslateCameraZ(bool wheelUp);
	void RotateCamera(const Vector3Int& position, const Vector3Int& prevPosition);
	void SetViewMatrix();
	void SetProjectionMatrix(float fov, float ratio, float nearZ, float farZ);

	const DirectX::XMMATRIX& GetViewMatrix() const;
	const DirectX::XMMATRIX& GetViewProjectionMatrix() const;

private:
	float m_speed;
	float m_angularSpeed;
	Vector3 m_position;
	Vector3 m_orientation;
	Vector3 m_up;
	Vector3Int m_mousePositionX, m_mousePositionY;
	DirectX::XMMATRIX m_viewMatrix;
	DirectX::XMMATRIX m_projectionMatrix;
	DirectX::XMMATRIX m_viewProjectionMatrix;
	CameraBuffer m_cameraBuffer;
	ConstantBufferVertex<CameraBuffer> m_constantBufferVertex;
	ConstantBufferPixel<CameraBuffer> m_constantBufferPixel;
};