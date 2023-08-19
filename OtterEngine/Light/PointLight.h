#pragma once

#include <DirectXMath.h>

#include "OtterEngine/Graphics/Graphics.h"
#include "OtterEngine/Graphics/Resource/ConstantBuffer.h"
#include "OtterEngine/Entity/DebugEntity/DebugSphere.h"
#include "OtterEngine/Common/Vertex.h"

class PointLight {
private:
	struct LightBuffer {
		DirectX::XMFLOAT4 m_position;
		Color4 m_color;
	};

public:
	PointLight(const Graphics& graphics, const DirectX::XMFLOAT4& position, const Color4& color, float scale,
		const DirectX::XMMATRIX& viewProjectionMatrix);

	void Update(const Graphics& graphics) const;
	void Render(const Graphics& graphics);
	void ShowControlWindow();

private:
	LightBuffer m_lightBuffer;
	float m_scale;
	ConstantBufferVertex<LightBuffer> m_constantBufferVertex;
	ConstantBufferPixel<LightBuffer> m_constantBufferPixel;
	DebugSphere m_sphereMesh;
};