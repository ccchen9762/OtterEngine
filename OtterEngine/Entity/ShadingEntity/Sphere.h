#pragma once

#include "../Entity.h"

class Sphere : public Entity
{
	friend class Entity;

public:
	Sphere(const Game& game, const Graphics& graphics, const Vector3& translation, const Vector3& rotation, const Vector3& scale,
		bool isStatic);
	~Sphere()=default;

private:
	void GenerateMesh(int division);

private:
	Attributes m_attributes;

	static std::vector<Vertex> s_vertices;
	static std::vector<unsigned short> s_indices;
};