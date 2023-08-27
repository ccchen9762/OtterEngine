#pragma once

#include "GraphicsResource.h"

class IndexBuffer : public GraphicsResource
{
public:
	IndexBuffer(const Graphics& graphics, const std::vector<unsigned short>& indices, const std::wstring& tag);
	~IndexBuffer() = default;

	static std::wstring GenerateUID(const std::vector<unsigned short>& indices, const std::wstring& tags) { return  L"IndexBuffer#" + tags; }

	void Bind(const Graphics& graphics) const override;

private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_pIndexBuffer;
};