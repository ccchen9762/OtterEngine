#pragma once

#include "../Entity.h"

class Cube : public Entity
{
	friend class Entity;

private:
	struct Attributes {
		alignas(16) float shiness;
	};

public:
	Cube(const Graphics& graphics, const Vector3& translation, const Vector3& rotation, const Vector3& scale,
		const Camera& camera, bool isStatic);
	~Cube() = default;

private:
	void static GenerateMesh();
	const std::vector<std::unique_ptr<GraphicsResource>>& GetCommonResources() const override;

private:
	Attributes m_attributes;

	static std::vector<Vertex> s_vertices;
	static std::vector<unsigned short> s_indices;

	static std::vector<std::unique_ptr<GraphicsResource>> s_commonResources;
};