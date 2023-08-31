#pragma once

#include "OtterEngine/Entity/DebugEntity/DebugSphere.h"
#include "OtterEngine/Common/constants.h"

class PointLight {
private:
	struct LightBuffer {
		int total = 0;
		int padding[3];
		DirectX::XMFLOAT4 positions[kMaxLight];
		Color4 colors[kMaxLight];
		Color4 ambient = { 0.15f, 0.15f, 0.15f, 1.0f };
		float intensity = 1.0f;
		float attenuationConst = 1.0f;
		float attenuationLinear = 0.007f;
		float attenuationQuadratic = 0.0002f;
	};

public:
	PointLight(const Graphics& graphics);

	void AddLight(const Game* game, const Graphics& graphics, const DirectX::XMFLOAT4& position, const Color4& color);
	void Update(const Graphics& graphics);
	void Render(const Graphics& graphics) const;
	void ShowControlWindow();

private:
	LightBuffer m_lightBuffer;
	ConstantBufferVertex<LightBuffer> m_pointBufferVertex;
	ConstantBufferPixel<LightBuffer> m_pointBufferPixel;
	std::vector<DebugSphere> m_sphereMeshs;
};