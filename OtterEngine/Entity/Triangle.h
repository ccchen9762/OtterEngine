#pragma once

#include "Entity.h"

class Triangle : public Entity
{
public:
	Triangle(const Microsoft::WRL::ComPtr<ID3D11Device>& pDevice,
		Vector3 rotation, Vector3 translation, Vector3 revolution, Vector3 scale);
	~Triangle() = default;

private:
	static const std::vector<Vertex> s_vertices;
	static const std::vector<unsigned short> s_indices;

};