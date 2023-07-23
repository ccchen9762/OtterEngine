#pragma once

#include "OtterEngine/Common/MinimalWindows.h"
#include <d3d11.h> // only declarations, need definitions in dlls 

class Graphics
{
public:
	Graphics(HWND hWnd);
	~Graphics();

	// prevent copying
	Graphics(const Graphics&) = delete;
	Graphics& operator= (const Graphics&) = delete;

	void ClearBuffer(float red, float green, float blue);
	void Update();		// called each frame to update graphics

private:
	void PostUpdate();	// things to do after Update()

private:
	ID3D11Device* m_pDevice;
	ID3D11DeviceContext* m_pDeviceContext;
	IDXGISwapChain* m_pSwapChain;
	ID3D11RenderTargetView* m_pRenderTarget;
};