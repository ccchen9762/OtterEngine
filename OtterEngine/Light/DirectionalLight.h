#pragma once

#include <DirectXMath.h>

#include "OtterEngine/Graphics/Resource/ConstantBuffer.h"
#include "OtterEngine/Common/Vertex.h"
#include "OtterEngine/Common/constants.h"

class DirectionalLight {
private:
	struct LightBuffer {
		int total = 0;
		int padding[3];
		DirectX::XMFLOAT4 directions[kMaxLight];
		Color4 colors[kMaxLight];
		Color4 ambient = { 0.15f, 0.15f, 0.15f, 1.0f };
		float intensity = 1.0f;
		float attenuationConst = 1.0f;
		float attenuationLinear = 0.007f;
		float attenuationQuadratic = 0.0002f;
	};

public:
	DirectionalLight(const Graphics& graphics);
	~DirectionalLight() = default;

	void AddLight(const Graphics& graphics, const DirectX::XMFLOAT4& direction, const Color4& color);
	void Update(const Graphics& graphics);
	void ShowControlWindow();

private:
	LightBuffer m_lightBuffer;
	ConstantBufferVertex<LightBuffer> m_directionalBufferVertex;
	ConstantBufferPixel<LightBuffer> m_directionalBufferPixel;
};