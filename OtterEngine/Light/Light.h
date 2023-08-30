#pragma once

class Light
{
protected:
	struct LightBuffer {
		DirectX::XMFLOAT4 position;
		Color4 lightColor;
		Color4 ambient;
		float intensity;
		float attenuationConst;
		float attenuationLinear;
		float attenuationQuadratic;
	};
public:
	Light();
	~Light();

private:
	LightBuffer m_lightBuffer;
	ConstantBufferVertex<LightBuffer> m_constantBufferVertex;
	ConstantBufferPixel<LightBuffer> m_constantBufferPixel;
};