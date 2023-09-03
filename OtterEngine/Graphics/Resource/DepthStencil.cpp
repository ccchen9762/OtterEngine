#include "DepthStencil.h"

DepthStencil::DepthStencil(const Graphics& graphics, Mode mode) : GraphicsResource(GenerateUID(mode)){
	// z-buffer need to be created manually
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc = CD3D11_DEPTH_STENCIL_DESC{ CD3D11_DEFAULT{} };

	if (mode == Mode::Write) {
		depthStencilDesc.StencilEnable = true;
		depthStencilDesc.StencilWriteMask = 0xFF;
		depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
	}
	else if (mode == Mode::Mask) {
		depthStencilDesc.DepthEnable = true;
		depthStencilDesc.StencilEnable = true;
		depthStencilDesc.StencilWriteMask = 0xFF;
		depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_NOT_EQUAL;
		depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	}

	GetDevice(graphics)->CreateDepthStencilState(&depthStencilDesc, &m_pDepthStencilState);
}

void DepthStencil::Bind(const Graphics& graphics) const {
	GetDeviceContext(graphics)->OMSetDepthStencilState(m_pDepthStencilState.Get(), 0xFF);
}
