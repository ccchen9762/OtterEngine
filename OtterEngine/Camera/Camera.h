#pragma once

#include "OtterEngine/Graphics/Resource/ConstantBuffer.h"
#include "OtterEngine/Common/Math/MathUtils.h"

class Camera
{
private:
	struct CameraBuffer {
		DirectX::XMVECTOR position;
	};

public:
	Camera(const Graphics& graphics, const Vector3& position, const Vector3& orientation, const Vector3& up);
	~Camera() = default;
	
	// called every frame
	void Update(const Graphics& graphics);

	void TranslateCamera(float translateX, float translateY);
	void TranslateCameraZ(int translateZ);
	void RotateCamera(float rotationY, float rotationX);
	void SetProjectionMatrix(float fov, float ratio, float nearZ, float farZ);

	const DirectX::XMMATRIX& GetViewMatrix() const { return m_viewMatrix; }
	const DirectX::XMMATRIX& GetViewProjectionMatrix() const { return m_viewProjectionMatrix; }

private:
	void SetViewMatrix();

private:
	Vector3 m_position;
	Vector3 m_orientation;
	Vector3 m_up;
	float m_speed;
	float m_angularSpeed;
	DirectX::XMMATRIX m_viewMatrix;
	DirectX::XMMATRIX m_projectionMatrix;
	DirectX::XMMATRIX m_viewProjectionMatrix;
	CameraBuffer m_cameraBuffer;
	ConstantBufferVertex<CameraBuffer> m_constantBufferVertex;
	ConstantBufferPixel<CameraBuffer> m_constantBufferPixel;
};