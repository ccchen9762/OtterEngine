#pragma once

#include "OtterEngine/Graphics/Graphics.h"

class GraphicsResource
{
public:
	virtual ~GraphicsResource() = default; // make sure derived class destructors are called properly
	virtual void Bind(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContext) const = 0;
};