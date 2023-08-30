#pragma once

#include "Light.h"

#include "OtterEngine/Entity/DebugEntity/DebugSphere.h"

class PointLight : Light {
public:
	PointLight(const Game& game, const Graphics& graphics, const DirectX::XMFLOAT4& position, const Color4& color);

	void Update(const Graphics& graphics);
	void Render(const Graphics& graphics) const;
	void ShowControlWindow();

private:
	LightBuffer m_lightBuffer;
	ConstantBufferVertex<LightBuffer> m_constantBufferVertex;
	ConstantBufferPixel<LightBuffer> m_constantBufferPixel;
	DebugSphere m_sphereMesh;

	static int s_numLight;
};