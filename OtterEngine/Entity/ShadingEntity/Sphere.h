#pragma once

#include "../Entity.h"

class Sphere : public Entity
{
	friend class Entity;

public:
	Sphere(const Graphics& graphics, const Vector3& translation, const Vector3& rotation, const Vector3& scale,
		const Camera& camera, bool isStatic);
	~Sphere()=default;

private:
	void GenerateMesh(int division);
	const std::vector<std::unique_ptr<GraphicsResource>>& GetCommonResources() const override;

private:
	static std::vector<Vertex> s_vertices;
	static std::vector<unsigned short> s_indices;

	static std::vector<std::unique_ptr<GraphicsResource>> s_commonResources;
};