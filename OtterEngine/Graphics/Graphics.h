#pragma once

#include "OtterEngine/Common/pch.h"
#include <d3d11.h> // only declarations, need definitions in dlls 
#include <memory>

#include "Resource/GraphicsResource.h"

class Graphics
{
	friend class Game;

public:
	Graphics(HWND hWnd, unsigned int viewportWidth, unsigned int viewportHeight);
	~Graphics() = default; // no need to call release for ComPtr

	// prevent copying
	Graphics(const Graphics&) = delete;
	Graphics& operator= (const Graphics&) = delete;

	void ClearBuffer(float red, float green, float blue);
	void Update();		// called each frame to update graphics
	void PostUpdate();	// things to do after Update()

private:
	const Microsoft::WRL::ComPtr<ID3D11Device>& GetDevice() { return m_pDevice; }
	const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& GetDeviceConetxt() { return m_pDeviceContext; }

private:
 	Microsoft::WRL::ComPtr<ID3D11Device> m_pDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_pDeviceContext;
	Microsoft::WRL::ComPtr<IDXGISwapChain> m_pSwapChain;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_pRenderTargetView;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_pDepthStencilView;
	
	D3D11_VIEWPORT m_viewport;
};