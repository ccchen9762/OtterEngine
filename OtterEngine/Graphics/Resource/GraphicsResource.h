#pragma once

#include "OtterEngine/Graphics/Graphics.h"

class GraphicsResource
{
public:
	virtual void Bind(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContext) const = 0;

protected:
	//static ID3D11Device* GetDevice(const Graphics& graphics) { return graphics.m_pDevice.Get(); }
	//static ID3D11DeviceContext* GetDeviceContext(const Graphics& graphics) { return graphics.m_pDeviceContext.Get(); }
};