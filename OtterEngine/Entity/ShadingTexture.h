#pragma once

#include "Entity.h"

class ShadingTexture : public Entity
{
	friend class Entity;

public:
	struct Attributes {
		alignas(16) float shiness;
		BOOL hasSpecularMap;
	};

public:
	ShadingTexture(const Graphics& graphics, const Vector3& translation, const Vector3& rotation, const Vector3& scale,
		size_t indiceSize, const Camera& camera, bool isStatic, Attributes attributes);
	virtual ~ShadingTexture() = default;

private:
	const std::vector<std::unique_ptr<GraphicsResource>>& GetShadingResources() const override { return s_shadingResources; }

protected:
	Attributes m_attributes;

private:
	static std::vector<std::unique_ptr<GraphicsResource>> s_shadingResources;
};