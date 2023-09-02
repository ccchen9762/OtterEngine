#include "Rasterizer.h"

Rasterizer::Rasterizer(const Graphics& graphics, bool cullBackface) : 
	GraphicsResource(GenerateUID(cullBackface)), m_cullBackface(cullBackface) {

	D3D11_RASTERIZER_DESC rasterizerDesc = {};
    rasterizerDesc.FillMode = D3D11_FILL_SOLID; // D3D11_FILL_WIREFRAME
    rasterizerDesc.CullMode = cullBackface ? D3D11_CULL_BACK : D3D11_CULL_NONE;
    /*rasterizerDesc.FrontCounterClockwise = true;
    rasterizerDesc.DepthBias = false;
    rasterizerDesc.DepthBiasClamp = 0;
    rasterizerDesc.SlopeScaledDepthBias = 0;
    rasterizerDesc.DepthClipEnable = true;
    rasterizerDesc.ScissorEnable = true;
    rasterizerDesc.MultisampleEnable = false;
    rasterizerDesc.AntialiasedLineEnable = false;*/

	DX::ThrowIfFailed(GetDevice(graphics)->CreateRasterizerState(&rasterizerDesc, &m_pRasterizerState));
}

void Rasterizer::Bind(const Graphics& graphics) const {
	GetDeviceContext(graphics)->RSSetState(m_pRasterizerState.Get());
}
