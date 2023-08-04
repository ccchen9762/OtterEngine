#pragma once

#include <DirectXMath.h>

#include "OtterEngine/Graphics/Resource/GraphicsResource.h"
#include "OtterEngine/Graphics/Resource/VertexShader.h"
#include "OtterEngine/Graphics/Resource/PixelShader.h"
#include "OtterEngine/Graphics/Resource/InputLayout.h"
#include "OtterEngine/Graphics/Resource/VertexBuffer.h"
#include "OtterEngine/Graphics/Resource/IndexBuffer.h"
#include "OtterEngine/Graphics/Resource/ConstantBuffer.h"
#include "OtterEngine/Common/Math/Vector3.h"

class Entity
{
public:
	Entity(Vector3 rotation, Vector3 translation, Vector3 revolution, Vector3 scale, size_t indicesSize);
	~Entity() = default;

	void Render(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContext);

	DirectX::XMMATRIX GetTransformMatrix() { 
		return DirectX::XMMatrixRotationRollPitchYaw(m_rotation.x, m_rotation.y, m_rotation.z) *
			DirectX::XMMatrixTranslation(m_translation.x, m_translation.y, m_translation.z) *
			//DirectX::XMMatrixRotationRollPitchYaw(m_revolution.x, m_revolution.y, m_revolution.z) *
			DirectX::XMMatrixScaling(m_scale.x, m_scale.y, m_scale.z);
	}

protected:
	Vector3 m_rotation;
	Vector3 m_translation;
	Vector3 m_revolution;
	Vector3 m_scale;
	size_t m_indicesSize;
	std::vector<std::unique_ptr<GraphicsResource>> m_graphicsResources;
	std::vector<std::unique_ptr<GraphicsResource>> m_graphicsBuffers;
};