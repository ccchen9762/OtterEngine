#pragma once

#include "GraphicsResource.h"

class AlphaBlender : public GraphicsResource
{
public:
	AlphaBlender(const Graphics& graphics, bool enableAlphaBlend);
	~AlphaBlender() = default;

	static std::wstring GenerateUID(bool enableAlphaBlend) { return  L"AlphaBlender#" + std::to_wstring(enableAlphaBlend); }

	void Bind(const Graphics& graphics) const override;

private:
	Microsoft::WRL::ComPtr<ID3D11BlendState> m_pBlenderState;
	bool m_enableAlphaBlend;
};