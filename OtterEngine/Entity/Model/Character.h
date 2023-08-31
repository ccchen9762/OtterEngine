#pragma once

#include "Model.h"

class Character : public Model
{
public:
	Character(const Game* game, const Graphics& graphics, const Vector3& translation, const Vector3& rotation, const Vector3& scale,
		bool isStatic, const std::string& path);
	~Character() = default;

private:
	static MeshInformation s_meshInformation;
};