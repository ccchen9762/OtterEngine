#pragma once

#include "../Entity.h"

class Plane : public Entity
{
	friend class Entity;

public:
	Plane(const Graphics& graphics, const Vector3& translation, const Vector3& rotation, const Vector3& scale,
		const Camera& camera, const std::wstring& path, bool isStatic);
	~Plane() = default;

private:
	Attributes m_attributes;

	static const std::vector<VertexTexture> s_vertices;
	static const std::vector<unsigned short> s_indices;
};