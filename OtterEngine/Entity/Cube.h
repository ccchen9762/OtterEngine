#pragma once

#include "Entity.h"

class Cube : public Entity
{
	friend class Entity;

public:
	Cube(const Graphics& graphics, const Vector3& translation, const Vector3& rotation, const Vector3& scale,
		const DirectX::XMMATRIX& viewProjectionMatrix, bool isStatic);
	~Cube() = default;

private:
	void static GenerateVertices();
	const std::vector<std::unique_ptr<GraphicsResource>>& GetCommonResources() const override;

private:
	static std::vector<Vertex> s_vertices;
	static std::vector<unsigned short> s_indices;

	static std::vector<std::unique_ptr<GraphicsResource>> s_commonResources;
};