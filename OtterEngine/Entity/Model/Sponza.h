#pragma once

#include "Model.h"

class Sponza : public Model
{
public:
	Sponza(const Game* game, const Graphics& graphics, const Vector3& translation, const Vector3& rotation, const Vector3& scale,
		bool isStatic, const std::string& path);
	~Sponza() = default;

private:
	static MeshInformation s_meshInformation;
};