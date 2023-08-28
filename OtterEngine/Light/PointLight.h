#pragma once

#include "OtterEngine/Entity/DebugEntity/DebugSphere.h"

class PointLight {
private:
	struct LightBuffer {
		DirectX::XMFLOAT4 position;
		Color4 lightColor;
		Color4 ambient;
		float diffuseIntensity;
		float attenuationConst;
		float attenuationLinear;
		float attenuationQuadratic;
	};

public:
	PointLight(const Game& game, const Graphics& graphics, const DirectX::XMFLOAT4& position, const Color4& color, float scale);

	void Update(const Graphics& graphics);
	void Render(const Graphics& graphics) const;
	void ShowControlWindow();

private:
	LightBuffer m_lightBuffer;
	float m_scale;
	ConstantBufferVertex<LightBuffer> m_constantBufferVertex;
	ConstantBufferPixel<LightBuffer> m_constantBufferPixel;
	DebugSphere m_sphereMesh;

	static int s_numLight;
};