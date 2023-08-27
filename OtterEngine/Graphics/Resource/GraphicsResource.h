#pragma once

#include <string> 

#include <assert.h>

#include "OtterEngine/Graphics/Graphics.h"

class GraphicsResource
{
public:
	GraphicsResource(const std::wstring& UID) : m_UID(UID) {}
	virtual ~GraphicsResource() = default; // make sure derived class destructors are called properly
	
	const std::wstring& GetUID() const { 
		assert("Graphics resource UID not generated." && !m_UID.empty());
		return m_UID;
	}

	virtual void Bind(const Graphics& graphics) const = 0;
	
protected:
	const Microsoft::WRL::ComPtr<ID3D11Device>& GetDevice(const Graphics& graphics) const { return graphics.m_pDevice; }
	const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& GetDeviceContext(const Graphics& graphics) const { return graphics.m_pDeviceContext; }

private:
	std::wstring m_UID;
};