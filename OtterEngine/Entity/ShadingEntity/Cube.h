#pragma once

#include "../Entity.h"

class Cube : public Entity
{
	friend class Entity;

public:
	Cube(const Game* game, const Graphics& graphics, const Vector3& translation, const Vector3& rotation, const Vector3& scale,
		bool isStatic);
	~Cube() = default;

	void DrawOutLine(const Graphics& graphics);

private:
	void static GenerateMesh();
	void static GenerateOutline(Color4 outlineColor);

private:
	Attributes m_attributes;

	static std::vector<Vertex> s_vertices;
	static std::vector<Vertex> s_outlineVertices;
	static std::vector<unsigned short> s_indices;

	std::vector<std::shared_ptr<GraphicsResource>> m_outlineResources;
};