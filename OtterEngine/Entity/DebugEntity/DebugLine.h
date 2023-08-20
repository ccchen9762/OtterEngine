#pragma once

#include "../Entity.h"

class DebugLine : public Entity
{
	friend class Entity;

public:
	DebugLine(const Graphics& graphics, const Vector3& translation, const Vector3& rotation, const Vector3& scale,
		const Camera& camera, const Color4& color, bool isStatic);
	~DebugLine() = default;

private:
	const std::vector<std::unique_ptr<GraphicsResource>>& GetCommonResources() const override;

private:
	std::vector<VertexBasic> m_vertices;
	static const std::vector<unsigned short> s_indices;

	static std::vector<std::unique_ptr<GraphicsResource>> s_commonResources;
};