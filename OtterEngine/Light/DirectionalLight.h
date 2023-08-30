#pragma once

#include "Light.h"

#include <DirectXMath.h>

#include "OtterEngine/Graphics/Resource/ConstantBuffer.h"
#include "OtterEngine/Common/Vertex.h"

class Game;

class DirectionalLight : Light {
public:
	DirectionalLight(const Game& game, const Graphics& graphics, const DirectX::XMFLOAT4& position, const Color4& color);

	void Update(const Graphics& graphics);
	void ShowControlWindow();
};