#pragma once

#include "OtterEngine/Graphics/Graphics.h"

class GraphicsResource
{
public:
	virtual ~GraphicsResource() = default; // make sure derived class destructors are called properly
	virtual void Bind(const Graphics& graphics) const = 0;

protected:
	const Microsoft::WRL::ComPtr<ID3D11Device>& GetDevice(const Graphics& graphics) const { return graphics.m_pDevice; }
	const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& GetDeviceContext(const Graphics& graphics) const { return graphics.m_pDeviceContext; }
};