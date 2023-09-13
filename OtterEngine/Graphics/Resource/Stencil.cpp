#include "Stencil.h"

Stencil::Stencil(const Graphics& graphics, Mode mode) : GraphicsResource(GenerateUID(mode)){

	D3D11_DEPTH_STENCIL_DESC depthStencilDesc = CD3D11_DEPTH_STENCIL_DESC{ CD3D11_DEFAULT{} };

	if (mode == Mode::Write) {
		depthStencilDesc.StencilEnable = true;
		depthStencilDesc.StencilWriteMask = 0xFF;
		depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
		depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_REPLACE;
	}
	else if (mode == Mode::Mask) {
		depthStencilDesc.DepthEnable = false;
		depthStencilDesc.StencilEnable = true;
		depthStencilDesc.StencilWriteMask = 0xFF;
		depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_NOT_EQUAL;
		depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	}
	else if (mode == Mode::Off) {
		// z-buffer need to be created manually
		depthStencilDesc.DepthEnable = true;
		depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
		depthStencilDesc.StencilEnable = false;	// skip all stencil stuff
	}

	GetDevice(graphics)->CreateDepthStencilState(&depthStencilDesc, &m_pStencilState);
}

void Stencil::Bind(const Graphics& graphics) const {
	GetDeviceContext(graphics)->OMSetDepthStencilState(m_pStencilState.Get(), 0xFF);
}
