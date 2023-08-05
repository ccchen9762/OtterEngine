#pragma once

#include "Entity.h"

#include "OtterEngine/Graphics/Camera/Camera.h"

class Cube : public Entity
{
	friend class Entity;

public:
	Cube(const Graphics& graphics, Vector3 rotation, Vector3 translation, Vector3 revolution, Vector3 scale, float speed = 1.0f);
	~Cube() = default;

private:
	const std::vector<std::unique_ptr<GraphicsResource>>& GetCommonResources() const override;

private:
	static const std::vector<Vertex> s_vertices;
	static const std::vector<unsigned short> s_indices;

	static std::vector<std::unique_ptr<GraphicsResource>> s_commonResources;
};