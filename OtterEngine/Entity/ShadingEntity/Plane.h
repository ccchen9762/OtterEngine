#pragma once

#include "../Entity.h"

class Plane : public Entity
{
	friend class Entity;

public:
	Plane(const Game* game, const Graphics& graphics, const Vector3& translation, const Vector3& rotation, const Vector3& scale,
		const std::wstring& path, const std::wstring& pathNormalMap, bool isStatic);
	~Plane() = default;

private:
	Attributes m_attributes;

	static const std::vector<VertexTexture> s_vertices;
	static const std::vector<unsigned short> s_indices;
};