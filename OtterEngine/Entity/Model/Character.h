#pragma once

#include "../ShadingEntity.h"

#include <string>

class Character : public ShadingEntity
{
	friend class Entity;

public:
	Character(const Graphics& graphics, const Vector3& translation, const Vector3& rotation, const Vector3& scale,
		const Camera& camera, const std::string& path, bool isStatic);
	~Character() = default;

private:
	static void LoadMesh();
	const std::vector<std::unique_ptr<GraphicsResource>>& GetCommonResources() const override { return s_commonResources; }

private:
	static std::string s_modelPath;

	static std::vector<Vertex> s_vertices;
	static std::vector<unsigned short> s_indices;

	static std::vector<std::unique_ptr<GraphicsResource>> s_commonResources;
};