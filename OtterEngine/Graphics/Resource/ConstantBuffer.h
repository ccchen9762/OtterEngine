#pragma once

#include "GraphicsResource.h"

#include <DirectXMath.h>

#include "OtterEngine/Entity/Entity.h"

struct CBuffer {
	DirectX::XMMATRIX transform;
};

class ConstantBufferTransformation : public GraphicsResource
{
public:
	ConstantBufferTransformation(const Graphics& graphics, const Entity& parentEntity);
	~ConstantBufferTransformation() = default;

	void Bind(const Graphics& graphics) const override;

private:
	void Update(const Graphics& graphics) const;

private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_pConstantBuffer;
	const Entity& m_parentEntity;
};

// ========================================

class ConstantBufferVertex : public GraphicsResource
{
public:
	ConstantBufferVertex(const Graphics& graphics);
	~ConstantBufferVertex() = default;

	void Bind(const Graphics& graphics) const override;

private:
	void Update(const Graphics& graphics) const;

private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_pConstantBuffer;
};

// ========================================

class ConstantBufferPixel : public GraphicsResource
{
public:
	ConstantBufferPixel(const Graphics& graphics);
	~ConstantBufferPixel() = default;

	void Bind(const Graphics& graphics) const override;

private:
	void Update(const Graphics& graphics) const;

private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_pConstantBuffer;
};