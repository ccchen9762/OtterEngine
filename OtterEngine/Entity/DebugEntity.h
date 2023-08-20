#pragma once

#include "Entity.h"

class DebugEntity : public Entity
{
	friend class Entity;

public:
	DebugEntity(const Graphics& graphics, const Vector3& translation, const Vector3& rotation, const Vector3& scale, 
		size_t indiceSize, const Camera& camera, bool isStatic);
	virtual ~DebugEntity() = default;

	const std::vector<std::unique_ptr<GraphicsResource>>& GetShadingResources() const override { return s_shadingResources; }

private:
	static std::vector<std::unique_ptr<GraphicsResource>> s_shadingResources;
};