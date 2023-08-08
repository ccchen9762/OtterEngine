#pragma once

#include "Entity.h"

class Triangle : public Entity
{
	friend class Entity;

public:
	Triangle(const Graphics& graphics, const Vector3& translation, const Vector3& rotation, const Vector3& scale,
		const DirectX::XMMATRIX& viewProjectionMatrix, bool isStatic);
	~Triangle() = default;

private:
	const std::vector<std::unique_ptr<GraphicsResource>>& GetCommonResources() const override;

private:
	static const std::vector<Vertex> s_vertices;
	static const std::vector<unsigned short> s_indices;

	static std::vector<std::unique_ptr<GraphicsResource>> s_commonResources;
};