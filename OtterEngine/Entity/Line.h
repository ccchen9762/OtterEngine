#pragma once

#include "Entity.h"

class Line : public Entity
{
	friend class Entity;

public:
	Line(const Graphics& graphics, const Vector3& translation, const Vector3& rotation, const Vector3& scale,
		const DirectX::XMMATRIX& viewProjectionMatrix, const Color4& color, bool isStatic);
	~Line() = default;

private:
	const std::vector<std::unique_ptr<GraphicsResource>>& GetCommonResources() const override;

private:
	std::vector<VertexSimple> m_vertices;
	static const std::vector<unsigned short> s_indices;

	static std::vector<std::unique_ptr<GraphicsResource>> s_commonResources;
};