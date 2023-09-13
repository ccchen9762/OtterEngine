#pragma once

#include <DirectXMath.h>
#include <vector>

#include "OtterEngine/Graphics/Resource/ConstantBuffer.h"
#include "OtterEngine/Common/constants.h"
#include "OtterEngine/Common/Vertex.h"

class Game;
class Graphics;
class Camera;
class DebugSphere;

namespace RG {
	class RenderGraph;
}

class PointLight {
private:
	struct LightBuffer {
		int total = 0;
		int padding[3];
		DirectX::XMFLOAT4 positions[kMaxLight];
		Color4 colors[kMaxLight];
		Color4 ambient = { 0.10f, 0.10f, 0.10f, 1.0f };
		float intensity = 1.0f;
		float attenuationConst = 1.0f;
		float attenuationLinear = 0.007f;
		float attenuationQuadratic = 0.0002f;
	};

public:
	PointLight(const Graphics& graphics);

	void AddLight(const Game* game, const Graphics& graphics, const DirectX::XMFLOAT4& position, const Color4& color);
	void Register(const RG::RenderGraph& renderGraph);
	void Update(const Graphics& graphics);
	void AssignJob() const;
	void ShowControlWindow();

private:
	LightBuffer m_lightBuffer;
	ConstantBufferVertex<LightBuffer> m_pointBufferVertex;
	ConstantBufferPixel<LightBuffer> m_pointBufferPixel;
	std::vector<DebugSphere> m_sphereMeshs;
	std::vector<Camera> m_cameras;
};