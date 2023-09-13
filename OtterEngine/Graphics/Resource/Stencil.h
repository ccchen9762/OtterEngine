#pragma once

#include "GraphicsResource.h"

class Stencil : public GraphicsResource
{
public:
	enum class Mode {
		Off = 0, 
		Write = 1,	// write information to stencil, but not mask out pixels
		Mask = 2	// mask out pixels with stencil
	};

public:
	Stencil(const Graphics& graphics, Mode mode);
	~Stencil() = default;
	
	static std::wstring GenerateUID(Mode mode) { return  L"Stencil#" + std::to_wstring(static_cast<int>(mode)); }
	
	void Bind(const Graphics& graphics) const override;

private:
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> m_pStencilState;
};