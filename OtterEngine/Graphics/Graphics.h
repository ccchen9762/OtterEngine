#pragma once

#include "OtterEngine/Common/pch.h"
#include <d3d11.h> // only declarations, need definitions in dlls 

class Graphics
{
public:
	Graphics(HWND hWnd);
	~Graphics() = default; // no need to call release for ComPtr

	// prevent copying
	Graphics(const Graphics&) = delete;
	Graphics& operator= (const Graphics&) = delete;

	void ClearBuffer(float red, float green, float blue);
	void Update();		// called each frame to update graphics

private:
	void PostUpdate();	// things to do after Update()

	void DrawTestTriangle();

private:
 	Microsoft::WRL::ComPtr<ID3D11Device> m_pDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_pDeviceContext;
	Microsoft::WRL::ComPtr<IDXGISwapChain> m_pSwapChain;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_pRenderTarget;
};