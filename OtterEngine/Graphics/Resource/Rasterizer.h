#pragma once

#include "GraphicsResource.h"

class Rasterizer : public GraphicsResource
{
public:
	Rasterizer(const Graphics& graphics, bool cullBackface);
	~Rasterizer() = default;

	static std::wstring GenerateUID(bool cullBackface) { return  L"Rasterizer#" + std::to_wstring(cullBackface); }

	void Bind(const Graphics& graphics) const override;

private:
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> m_pRasterizerState;
	bool m_cullBackface;
};