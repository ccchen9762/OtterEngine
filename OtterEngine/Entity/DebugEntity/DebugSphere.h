#pragma once

#include "../Entity.h"

class DebugSphere : public Entity
{
	friend class Entity;

public:
	DebugSphere(const Game& game, const Graphics& graphics, const Vector3& translation, const Vector3& rotation, const Vector3& scale,
		bool isStatic);
	~DebugSphere() = default;

private:
	void GenerateMesh(int division);

private:
	static std::vector<VertexBasic> s_vertices;
	static std::vector<unsigned short> s_indices;
};