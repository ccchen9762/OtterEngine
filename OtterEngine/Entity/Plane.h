#pragma once

#include "Entity.h"

class Plane : public Entity
{
	friend class Entity;

public:
	Plane(const Graphics& graphics, const Vector3& translation, const Vector3& rotation, const Vector3& scale,
		const DirectX::XMMATRIX& viewProjectionMatrix, const std::wstring& path, bool isStatic);
	~Plane() = default;

private:
	const std::vector<std::unique_ptr<GraphicsResource>>& GetCommonResources() const override;

private:
	static const std::vector<VertexTexture> s_vertices;
	static const std::vector<unsigned short> s_indices;

	static std::vector<std::unique_ptr<GraphicsResource>> s_commonResources;
};