#pragma once

#include "../Entity.h"

class DebugSphere : public Entity
{
	friend class Entity;

public:
	DebugSphere(const Graphics& graphics, const Vector3& translation, const Vector3& rotation, const Vector3& scale,
		const Camera& camera, bool isStatic);
	~DebugSphere() = default;

private:
	void GenerateMesh(int division);
	const std::vector<std::unique_ptr<GraphicsResource>>& GetCommonResources() const override;

private:
	static std::vector<VertexBasic> s_vertices;
	static std::vector<unsigned short> s_indices;

	static std::vector<std::unique_ptr<GraphicsResource>> s_commonResources;
};