#include "AlphaBlender.h"

AlphaBlender::AlphaBlender(const Graphics& graphics, bool enableAlphaBlend) : 
	GraphicsResource(GenerateUID(enableAlphaBlend)), m_enableAlphaBlend(enableAlphaBlend) {
	
	D3D11_BLEND_DESC blendDesc = {};
	D3D11_RENDER_TARGET_BLEND_DESC& blendRenderTarget = blendDesc.RenderTarget[0]; // use reference!!
	if (enableAlphaBlend) {
		blendRenderTarget.BlendEnable = TRUE;
		blendRenderTarget.SrcBlend = D3D11_BLEND_SRC_ALPHA;
		blendRenderTarget.DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		blendRenderTarget.BlendOp = D3D11_BLEND_OP_ADD;
		blendRenderTarget.SrcBlendAlpha = D3D11_BLEND_ONE;
		blendRenderTarget.DestBlendAlpha = D3D11_BLEND_ZERO;
		blendRenderTarget.BlendOpAlpha = D3D11_BLEND_OP_ADD;
		blendRenderTarget.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	}
	else {
		blendRenderTarget.BlendEnable = FALSE;
		blendRenderTarget.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	}
	DX::ThrowIfFailed(GetDevice(graphics)->CreateBlendState(&blendDesc, &m_pBlenderState));
}

void AlphaBlender::Bind(const Graphics& graphics) const {
	GetDeviceContext(graphics)->OMSetBlendState(m_pBlenderState.Get(), nullptr, 0xFFFFFFFF);
}