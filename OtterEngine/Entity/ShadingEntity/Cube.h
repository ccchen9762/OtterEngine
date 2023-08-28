#pragma once

#include "../Entity.h"

class Cube : public Entity
{
	friend class Entity;

public:
	Cube(const Game& game, const Graphics& graphics, const Vector3& translation, const Vector3& rotation, const Vector3& scale,
		bool isStatic);
	~Cube() = default;

private:
	void static GenerateMesh();

private:
	Attributes m_attributes;

	static std::vector<Vertex> s_vertices;
	static std::vector<unsigned short> s_indices;
};