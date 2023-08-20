#pragma once

#include "Entity.h"

class ShadingEntity : public Entity
{
	friend class Entity;

protected:
	struct Attributes {
		alignas(16) float shiness;
	};

public:
	ShadingEntity(const Graphics& graphics, const Vector3& translation, const Vector3& rotation, const Vector3& scale,
		size_t indiceSize, const Camera& camera, bool isStatic);
	virtual ~ShadingEntity()=default;

private:
	const std::vector<std::unique_ptr<GraphicsResource>>& GetShadingResources() const override { return s_shadingResources; }

protected:
	Attributes m_attributes;

private:
	static std::vector<std::unique_ptr<GraphicsResource>> s_shadingResources;
};